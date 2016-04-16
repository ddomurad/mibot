#include "./Clients/RoverClientsProvider.h"
#include "./Forms/SensorsReaderForm.h"
#include "ui_SensorsReaderForm.h"


SensorsReaderForm::SensorsReaderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorsReaderForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Sensors reader");
    auto sensorClient = RoverClientsProvider::GetRoverSensorClient();
    connect(sensorClient, SIGNAL(newData(RoverSensors)), this, SLOT(onSensorData(RoverSensors)));
}

SensorsReaderForm::~SensorsReaderForm()
{
    delete ui;
}

void SensorsReaderForm::onSensorData(RoverSensors sensors)
{
    ui->tableWidget->clear();
    QStringList headers;
    headers << "Name" << "Value";


    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setItem(0,0, new QTableWidgetItem("DateTime"));
    ui->tableWidget->setItem(0,1, new QTableWidgetItem(sensors.systemSensors.datetime.toString("dd_MM_yy hh:mm:ss")));

    ui->tableWidget->setItem(1,0, new QTableWidgetItem("Acc"));
    ui->tableWidget->setItem(1,1, new QTableWidgetItem(QString("[%1,%2,%3]")
                    .arg(sensors.arduinoSensors.acc[0])
                    .arg(sensors.arduinoSensors.acc[1])
                    .arg(sensors.arduinoSensors.acc[2])));

    ui->tableWidget->setItem(2,0, new QTableWidgetItem("Mag"));
    ui->tableWidget->setItem(2,1, new QTableWidgetItem(QString("[%1,%2,%3]")
                    .arg(sensors.arduinoSensors.mag[0])
                    .arg(sensors.arduinoSensors.mag[1])
                    .arg(sensors.arduinoSensors.mag[2])));

    ui->tableWidget->setItem(3,0, new QTableWidgetItem("US"));
    ui->tableWidget->setItem(3,1, new QTableWidgetItem(QString::number(sensors.arduinoSensors.us)));
    QString analogString = "";

    for(int i=0;i<10;i++)
    {
        if(sensors.arduinoSensors.hasAnalogValue[i])
            analogString += QString("(%1:%2) ").arg(i).arg(sensors.arduinoSensors.analogValue[i]);
    }

    ui->tableWidget->setItem(4,0, new QTableWidgetItem("Analog"));
    ui->tableWidget->setItem(4,1, new QTableWidgetItem(analogString));

    ui->tableWidget->setItem(5,0, new QTableWidgetItem("CPU_TEMP"));
    ui->tableWidget->setItem(5,1, new QTableWidgetItem(QString::number(sensors.systemSensors.cpu_temp)));

    ui->tableWidget->setItem(6,0, new QTableWidgetItem("CPU"));
    ui->tableWidget->setItem(6,1, new QTableWidgetItem(QString("%1 / %2")
                                                       .arg(sensors.systemSensors.cpu_usage_server)
                                                       .arg(sensors.systemSensors.cpu_usage_total)));

    ui->tableWidget->setItem(7,0, new QTableWidgetItem("MEMORY"));
    ui->tableWidget->setItem(7,1, new QTableWidgetItem(QString("%1 / %2 / %3")
                                                       .arg(sensors.systemSensors.mem_usage_server)
                                                       .arg(sensors.systemSensors.mem_usage_total)
                                                       .arg(sensors.systemSensors.mem_available)));

    ui->tableWidget->setItem(8,0, new QTableWidgetItem("GPS_FIX"));
    ui->tableWidget->setItem(8,1, new QTableWidgetItem(sensors.gpsSensors.fix ? "YES" : "NO"));

    ui->tableWidget->setItem(9,0, new QTableWidgetItem("GPS_POSITION"));
    ui->tableWidget->setItem(9,1, new QTableWidgetItem(QString("Lat: %1, Lot: %2")
                                                       .arg(sensors.gpsSensors.latitude)
                                                       .arg(sensors.gpsSensors.lognitude)));

    ui->tableWidget->setItem(10,0, new QTableWidgetItem("GPS_MOVEMENT"));
    ui->tableWidget->setItem(10,1, new QTableWidgetItem(QString("Speed: %1[m/s], Course: %2")
                                                       .arg(sensors.gpsSensors.speedKmh*1000.0/60.0)
                                                       .arg(sensors.gpsSensors.course)));
}

