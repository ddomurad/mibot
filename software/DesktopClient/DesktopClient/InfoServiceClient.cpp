#include "AppSettings.h"
#include "LoggerDialog.h"
#include "InfoServiceClient.h"

InfoServiceClient::InfoServiceClient(QObject *parent) :
    QObject(parent)
{
    bool ssl = AppSettings::GetKey(AppSettings::Connection_Rover_Info_SSL).toBool();
    _socket = new Socket("Info service", ssl, this);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

InfoServiceClient::~InfoServiceClient()
{
    _socket->disconnect();
}

bool InfoServiceClient::Refresh()
{
    _isRefreshed = false;
    _infos.clear();

    int port = AppSettings::GetKey(AppSettings::Connection_Rover_Info_Port).toInt();

    if(!_socket->SucessfulConnected())
    {
           if(!_socket->ConnectToRover(port))
               return false;
            else
               LoggerDialog::get()->Info("Info service client", "Connected to rover info service");
    }

    QString sendStr("{\"get\":[\"sockets\"]}");

    _socket->write(sendStr.toUtf8());

    return true;
}

QList<ServiceInfo> InfoServiceClient::GetServiceInfos()
{
    return _infos;
}


void InfoServiceClient::onSocketReadyRead()
{
    auto byteArray = _socket->readAll();
    _jsonProtocol.PushData( byteArray );

    fixIfJsonIsCorrupted();

    while(_jsonProtocol.ContainsValidJSONObject())
    {
        auto obj = _jsonProtocol.GetPendingObject();
        processObject(obj);
        fixIfJsonIsCorrupted();
    }
}

void InfoServiceClient::fixIfJsonIsCorrupted()
{
    if(_jsonProtocol.IsDataCorupted())
    {
        LoggerDialog::get()->Warning("JSON Parser", "StatusStrategy JSONProtocol handler contains corrupted json data. The handler will try to remove corrupted data.");
        _jsonProtocol.RemoveCoruptedData();
    }
}

void InfoServiceClient::processObject(QJsonObject &object)
{
    QJsonArray sockets = object["sockets"].toArray();

    _infos.clear();
    for(int i=0; i<sockets.count();i++)
    {
        QJsonObject obj = sockets.at(i).toObject();
        ServiceInfo info;
        info.name = obj["name"].toString();
        info.type = obj["type"].toString();
        info.port = obj["port"].toInt();
        info.ssl = obj["ssl"].toBool();
        info.accessLevel = obj["privileges"].toInt();
        _infos.append(info);
    }

    _isRefreshed = true;
    _socket->close();
    emit onRefreshDone();
}
