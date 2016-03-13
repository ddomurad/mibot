#include "./Forms/LoggerDialog.h"
#include "./Settings/AppSettings.h"
#include "./Clients/InfoServiceClient.h"
#include "./Clients/RoverSensorClient.h"

RoverSensorClient::RoverSensorClient(QObject *parent) :
    RoverClient(parent)
{}

RoverSensorClient::~RoverSensorClient()
{

}

bool RoverSensorClient::Connect()
{
    return _startConnecting(AppSettings::GetKey(AppSettings::Connection_Status_Service).toString());
}

void RoverSensorClient::SetSoundSignal(bool state)
{
    if(state)
        SendData("{\"piezo\":true}");
    else
        SendData("{\"piezo\":false}");
}

void RoverSensorClient::onData(QByteArray data)
{
    protocol.PushData(data);
    fixIfJsonIsCorrupted();

    while(protocol.ContainsValidJSONObject())
    {
        QJsonObject obj = protocol.GetPendingObject();
        processObj(obj);
        fixIfJsonIsCorrupted();
    }
}

void RoverSensorClient::onConnection()
{
    QString data = "{\"send_trigger\":\"auto\"}";
    _socket->write(data.toUtf8());
}

void RoverSensorClient::processObj(QJsonObject &obj)
{
    if(obj["sensors"].isUndefined() || obj["system"].isUndefined())
    {
        LoggerDialog::get()->Error("Process sensor data", "Invalid json object");
        return;
    }

    QJsonObject sensorsObj = obj["sensors"].toObject();
    QJsonObject systemObj = obj["system"].toObject();
    QJsonObject gpsObj = obj["gps"].toObject();

    RoverSensors sensors;
    sensors.systemSensors.cpu_temp = systemObj["cpu_temp"].toDouble();
    sensors.systemSensors.cpu_usage_server = systemObj["cpu_usage_server"].toDouble();
    sensors.systemSensors.cpu_usage_total = systemObj["cpu_usage_total"].toDouble();
    sensors.systemSensors.mem_available = systemObj["mem_available"].toDouble();
    sensors.systemSensors.mem_usage_server = systemObj["mem_usage_server"].toDouble();
    sensors.systemSensors.mem_usage_total= systemObj["mem_usage_total"].toDouble();

    sensors.gpsSensors.fix = gpsObj["fix"].toInt() != 0;
    sensors.gpsSensors.latitude = gpsObj["latitude"].toDouble();
    sensors.gpsSensors.lognitude = gpsObj["lognitude"].toDouble();
    sensors.gpsSensors.course = gpsObj["cource"].toDouble();
    sensors.gpsSensors.speedKmh = gpsObj["speed_kmh"].toDouble();

     QJsonArray accArray = sensorsObj["acc"].toArray();
     QJsonArray magArray = sensorsObj["mag"].toArray();
     QJsonArray analogArray = sensorsObj["analog"].toArray();

     sensors.arduinoSensors.acc[0] = accArray.at(0).toDouble();
     sensors.arduinoSensors.acc[1] = accArray.at(1).toDouble();
     sensors.arduinoSensors.acc[2] = accArray.at(2).toDouble();

     sensors.arduinoSensors.mag[0] = magArray.at(0).toDouble();
     sensors.arduinoSensors.mag[1] = magArray.at(1).toDouble();
     sensors.arduinoSensors.mag[2] = magArray.at(2).toDouble();

     sensors.arduinoSensors.us = sensorsObj["us"].toInt();

     for(int i=0;i<10;i++)
        sensors.arduinoSensors.hasAnalogValue[i] = false;

     for(int i=0; i<analogArray.count(); i++)
     {
         QJsonObject analogObject = analogArray.at(i).toObject();
         int c = analogObject["c"].toInt();
         float v = analogObject["v"].toDouble();
         if(c < 0 || c >9)
         {
             LoggerDialog::get()->Error("Process sensor data", "Invalid analog channel");
             return;
         }

         sensors.arduinoSensors.analogValue[c] = v;
         sensors.arduinoSensors.hasAnalogValue[c] = true;
     }

     emit newData(sensors);
}

void RoverSensorClient::fixIfJsonIsCorrupted()
{
    if(protocol.IsDataCorupted())
    {
        LoggerDialog::get()->Warning("JSON Parser", "StatusStrategy JSONProtocol handler contains corrupted json data. The handler will try to remove corrupted data.");
        protocol.RemoveCoruptedData();
    }
}
