#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dstate->setText("");
    image.load(":/new/res/none.jpeg");
    pixmap = QPixmap::fromImage(image);
    ui->dstate->setPixmap(pixmap);

    apClient = new RoverAutopilotClient(this);
    scClient = new RoverSensorClient(this);

    ui->_osm->setResources(
                "http://otile1.mqcdn.com/tiles/1.0.0/osm/",
                "/home/work/Tmp/osm_cache",
                "jpeg",
                "/home/work/Projects/praca_mgr/mibot/software/DesktopClient/debug/missing.jpeg");

    ui->_osm->setZoomRange(5,18);
    ui->_osm->setZoom(15);
    ui->_osm->centreGPSPossition(QPointF(19.9229, 50.0767));
    ui->_osm->addMapMenu(123,"SetGoal");

    connect(ui->_osm, SIGNAL(menuAction(OSMMenuItem)), this, SLOT(onGPSMenu(OSMMenuItem)));
    connect(apClient, SIGNAL(StateUpdate(AutopilotState)), this , SLOT(apStateUpdate(AutopilotState)));
    connect(scClient, SIGNAL(newData(RoverSensors)), this, SLOT(newSensorData(RoverSensors)));

    _update_timer = new QTimer(this);
    connect(_update_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    _update_timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    apClient->Connect(ui->addr->text(), ui->port1->text().toInt());
    scClient->Connect(ui->addr->text(), ui->port2->text().toInt());
}

void MainWindow::onGPSMenu(OSMMenuItem m)
{
    if(m.id = 123)
    {
        _rover_goal = m.gpsPos;

        ui->lat->setText( QString::number(_rover_goal.x()) );
        ui->lot->setText( QString::number(_rover_goal.y()) );

        ui->_osm->removeAllRoutes();

        OSMRoute route;
        route.addPoint(_rover_goal,0);
        route.name = "goal";
        route.stdFirstMarkerType = OSMMarkerType::Triangle2;
        route.editable = false;

        OSMRoute route2;
        route2.addPoint(_rover_gps_pos,0);
        route2.name = "rover";
        route2.editable = false;
        route2.stdFirstMarkerType = OSMMarkerType::Square;

        ui->_osm->addRoute(route);
        ui->_osm->addRoute(route2);
    }
}

void MainWindow::newSensorData(RoverSensors data)
{
    if(!data.gpsSensors.fix)
    {
        ui->R_lat->setText("NO_GPS");
        ui->R_lot->setText("NO_GPS");
        return;
    }

    ui->R_lat->setText(QString::number(data.gpsSensors.latitude));
    ui->R_lot->setText(QString::number(data.gpsSensors.lognitude));

    OSMRoute *rover = ui->_osm->getRoute("rover");
    rover->getMarker(0)->possition.setX(data.gpsSensors.latitude);
    rover->getMarker(0)->possition.setY(data.gpsSensors.lognitude);

    ui->_osm->update();
}

void MainWindow::apStateUpdate(AutopilotState state)
{
    ui->angle->setText( QString::number(state.angle));
    ui->angle->setText( QString::number(state.dist));

    if(state.drive_type == "R")
        image.load(":/new/res/right.jpeg");
    else if(state.drive_type == "L")
        image.load(":/new/res/left.jpeg");
    else if(state.drive_type == "F")
        image.load(":/new/res/up.jpeg");
    else image.load(":/new/res/none.jpeg");

    pixmap = QPixmap::fromImage(image);
    ui->dstate->setPixmap(pixmap);
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    _ap_enabled = checked;
}

void MainWindow::onTimer()
{
    if(!apClient->IsConnected())
    {
        ui->checkBox->setEnabled(false);
        return;
    }

    if(!_ap_enabled)
    {
        apClient->SetAutopilot(false);
        return;
    }

    apClient->SetAutopilot( _rover_goal, 1, true);
}
