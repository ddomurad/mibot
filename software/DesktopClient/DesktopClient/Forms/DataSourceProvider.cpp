#include "./Clients/RoverClientsProvider.h"
#include "DataSourceProvider.h"

DataSourceProvider::DataSourceProvider() :
    QObject(nullptr)
{

    _timer = new QTimer(this);
    connect(_timer,SIGNAL(timeout()), this, SLOT(fakeTimer()));
    _timer->start(50);

    dataSources.append(new DataSource("acc_x", 250, -20, 20));
    dataSources.append(new DataSource("acc_y", 250, -20, 20));
    dataSources.append(new DataSource("acc_z", 250, -20, 20));
    dataSources.append(new DataSource("mag_x", 250, -17000, 17000));
    dataSources.append(new DataSource("mag_y", 250, -17000, 17000));
    dataSources.append(new DataSource("mag_z", 250, -17000, 17000));
    dataSources.append(new DataSource("us", 250, -1, 210));

    dataSources.append(new DataSource("cpu_temp", 250, 0, 60));
    dataSources.append(new DataSource("cpu_usage_server", 250, -0.2, 1));
    dataSources.append(new DataSource("cpu_usage_total", 250, -0.2, 1));
    dataSources.append(new DataSource("mem_available", 250, 0, 512));
    dataSources.append(new DataSource("mem_usage_server", 250, 0,512));
    dataSources.append(new DataSource("mem_usage_total", 250, 0, 512));


    dataSources.append(new DataSource("bat_1", 250, -1, 10));
    dataSources.append(new DataSource("bat_2", 250, -1, 10));

    dataSources.append(new DataSource("fake", 250, -1000, 1000));
    dataSources.append(new DataSource("fake_90", 250, -1000, 1000));


    RoverSensorClient * sensorClient = RoverClientsProvider::GetRoverSensorClient();
    connect(sensorClient, SIGNAL(newData(RoverSensors)), this, SLOT(onNewSensorData(RoverSensors)));
}

DataSourceProvider::~DataSourceProvider()
{
    for(DataSource * source : dataSources)
        delete source;
}

DataSourceProvider *DataSourceProvider::get()
{
    static DataSourceProvider provider;
    return &provider;
}

QList<QString> DataSourceProvider::DataSources()
{
    QList<QString> list;

    for(DataSource * source : dataSources)
        list.append(source->lable);

    return list;
}

DataSource *DataSourceProvider::GetSource(QString name)
{
    for(DataSource * source : dataSources)
        if(source->lable == name)
            return source;

    return nullptr;
}


void DataSourceProvider::onNewSensorData(RoverSensors readings)
{
    dataSources[0]->PushData(readings.arduinoSensors.acc[0]);
    dataSources[1]->PushData(readings.arduinoSensors.acc[1]);
    dataSources[2]->PushData(readings.arduinoSensors.acc[2]);

    dataSources[3]->PushData(readings.arduinoSensors.mag[0]);
    dataSources[4]->PushData(readings.arduinoSensors.mag[1]);
    dataSources[5]->PushData(readings.arduinoSensors.mag[2]);

    dataSources[6]->PushData(readings.arduinoSensors.us);

    dataSources[7]->PushData(readings.systemSensors.cpu_temp);
    dataSources[8]->PushData(readings.systemSensors.cpu_usage_server);
    dataSources[9]->PushData(readings.systemSensors.cpu_usage_total);
    dataSources[10]->PushData(readings.systemSensors.mem_available);
    dataSources[11]->PushData(readings.systemSensors.mem_usage_server);
    dataSources[12]->PushData(readings.systemSensors.mem_usage_total);

    dataSources[13]->PushData(readings.arduinoSensors.analogValue[0]);
    dataSources[14]->PushData(readings.arduinoSensors.analogValue[1]);


}

void DataSourceProvider::fakeTimer()
{
    static double t= 0;
    t += 0.1;

    dataSources[15]->PushData(100.0*sin(t));
    dataSources[16]->PushData(100.0*cos(t));
}

