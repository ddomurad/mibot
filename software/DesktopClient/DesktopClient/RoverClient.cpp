#include "AppSettings.h"
#include "RoverClient.h"
#include "LoggerDialog.h"

RoverClient::RoverClient(QObject *parent) : QObject(parent)
{
    _infoClient = new InfoServiceClient(this);
    connect(_infoClient, SIGNAL(onRefreshDone()), this, SLOT(on_service_info()));
}

RoverClient::~RoverClient()
{
    _socket->close();
}

bool RoverClient::IsConnected()
{
    if(_socket == nullptr)
        return false;

    return _socket->SucessfulConnected();
}

bool RoverClient::_startConnecting(QString _serviceName)
{
    if(_socket != nullptr && _socket->SucessfulConnected())
        _socket->close();
    _socket->deleteLater();
    _socket = nullptr;

    serviceName = _serviceName;
    _infoClient->Refresh();
}


void RoverClient::on_service_info()
{
    QList<ServiceInfo> infos = _infoClient->GetServiceInfos();
    for(ServiceInfo info : infos)
    {
        if(info.name == serviceName)
        {
            _socket = new Socket(serviceName,info.ssl, this);
            connect(_socket, SIGNAL(readyRead()), this, SLOT(on_socket_data()));

            if(!_socket->ConnectToRover(info.port))
            {
                LoggerDialog::get()->Error("Connect to service","Can't connect to : " + serviceName);
                return;
            }

            LoggerDialog::get()->Info("Connect to service","Connected to : " + serviceName);
            return;
        }
    }

    LoggerDialog::get()->Error("Connect to service","Can't find info about service: " + serviceName);
}

void RoverClient::on_socket_data()
{
    onData(_socket->readAll());
}


