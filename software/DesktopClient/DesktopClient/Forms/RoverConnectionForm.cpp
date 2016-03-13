#include "./Clients/RoverClientsProvider.h"
#include "./Forms/RoverConnectionForm.h"
#include "ui_RoverConnectionForm.h"

RoverConnectionForm::RoverConnectionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoverConnectionForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Connections");
    _updateTimer = new QTimer(this);
    _updateTimer->setInterval(1000);

    connect(_updateTimer, SIGNAL(timeout()), this, SLOT(on_update()));
    _updateTimer->start();    
}

RoverConnectionForm::~RoverConnectionForm()
{
    delete ui;
}

void RoverConnectionForm::on_update()
{
    bool sensorsConnected = RoverClientsProvider::GetRoverSensorClient()->IsConnected();
    bool driveConnected = RoverClientsProvider::GetRoverDriveClient()->IsConnected();
    bool videoConnected = RoverClientsProvider::GetVideoServiceClient()->IsConnected();

    ui->label_sensors_state->setText( sensorsConnected ? "connected" : "disconnected");
    ui->label_drive_state->setText( driveConnected ? "connected" : "disconnected");
    ui->label_video_state->setText( videoConnected ? "connected" : "disconnected");
}


void RoverConnectionForm::on_pushButton_disconnect_all_clicked()
{
    RoverClientsProvider::GetRoverSensorClient()->Disconnect();
    RoverClientsProvider::GetRoverDriveClient()->Disconnect();
    RoverClientsProvider::GetVideoServiceClient()->Disconnect();
}

void RoverConnectionForm::on_pushButton_connect_all_clicked()
{
    RoverClientsProvider::GetRoverSensorClient()->Connect();
    RoverClientsProvider::GetRoverDriveClient()->Connect();
    RoverClientsProvider::GetVideoServiceClient()->Connect();
}

void RoverConnectionForm::on_pushButton_disconnect_video_clicked()
{
    RoverClientsProvider::GetVideoServiceClient()->Disconnect();
}

void RoverConnectionForm::on_pushButton_connect_video_clicked()
{
    RoverClientsProvider::GetVideoServiceClient()->Connect();
}

void RoverConnectionForm::on_pushButton_connect_drive_clicked()
{
    RoverClientsProvider::GetRoverDriveClient()->Connect();
}

void RoverConnectionForm::on_pushButton_disconnect_driver_clicked()
{
    RoverClientsProvider::GetRoverDriveClient()->Disconnect();
}

void RoverConnectionForm::on_pushButton_con_sensors_clicked()
{
    RoverClientsProvider::GetRoverSensorClient()->Connect();
}

void RoverConnectionForm::on_pushButton_disconnect_sensors_clicked()
{
    RoverClientsProvider::GetRoverSensorClient()->Disconnect();
}
