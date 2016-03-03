#include "LoggerDialog.h"
#include "AppSettings.h"
#include "InfoServiceClient.h"
#include "RoverSensorClient.h"

RoverSensorClient::RoverSensorClient(QObject *parent) : RoverClient(parent)
{}

RoverSensorClient::~RoverSensorClient()
{

}

bool RoverSensorClient::Connect()
{
    return _startConnecting(AppSettings::GetKey(AppSettings::Connection_Status_Service).toString());
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

void RoverSensorClient::processObj(QJsonObject &obj)
{

}

void RoverSensorClient::fixIfJsonIsCorrupted()
{
    if(protocol.IsDataCorupted())
    {
        LoggerDialog::get()->Warning("JSON Parser", "StatusStrategy JSONProtocol handler contains corrupted json data. The handler will try to remove corrupted data.");
        protocol.RemoveCoruptedData();
    }
}
