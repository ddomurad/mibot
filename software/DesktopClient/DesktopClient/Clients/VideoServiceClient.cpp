#include "./Settings/AppSettings.h"
#include "VideoServiceClient.h"
#include "./Forms/LoggerDialog.h"

VideoServiceClient::VideoServiceClient(QObject *parent) : RoverClient(parent)
{    
}

VideoServiceClient::~VideoServiceClient()
{

}

bool VideoServiceClient::Connect()
{
    return _startConnecting(AppSettings::GetKey(AppSettings::Connection_Video_Service).toString());
}

void VideoServiceClient::onData(QByteArray data)
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

void VideoServiceClient::onConnection()
{

}

QString VideoServiceClient::clientName()
{
    return "VideoServiceClient";
}

void VideoServiceClient::StartStreamer(int w, int h, QString device, QString serverAddr, int serverPort, QString format, int buffer)
{
    QString data = QString("{ \
    \"stream\":\"enable\",\
    \"device\":\"%1\",\
    \"addr\":\"%2\",\
    \"port\":%3,\
    \"width\":%4,\
    \"height\":%5,\
    \"buffers\":%6,\
    \"format\":\"%7\"\
    }");

    data = data.arg(device);
    data = data.arg(serverAddr);
    data = data.arg(serverPort);
    data = data.arg(w);
    data = data.arg(h);
    data = data.arg(buffer);
    data = data.arg(format);

    SendData(data);
}

void VideoServiceClient::StopStreamer()
{
    SendData("{\"stream\":\"disable\"}");
}


void VideoServiceClient::processObj(QJsonObject &obj)
{

}

void VideoServiceClient::fixIfJsonIsCorrupted()
{
    if(protocol.IsDataCorupted())
    {
        LoggerDialog::get()->Warning("JSON Parser", "StatusStrategy JSONProtocol handler contains corrupted json data. The handler will try to remove corrupted data.");
        protocol.RemoveCoruptedData();
    }
}

