#include "./Settings/AppSettings.h"
#include "./Clients/RoverClient.h"
#include "./Forms/LoggerDialog.h"
#include "./Other/Recorder.h"

RoverClient::RoverClient(QObject *parent) :
    QObject(parent),
    _socket(nullptr)
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

void RoverClient::Disconnect()
{
    if(_socket == nullptr)
        return;

    _socket->close();
    _socket->deleteLater();
    _socket = nullptr;
}


bool RoverClient::SendData(uchar *data, int size)
{
    if(!IsConnected())
    {
        LoggerDialog::get()->Error("Send Data", "No connection");
        return false;
    }

    int cnt = _socket->write((char*)data, size);
    if(cnt == size)
        return true;

    LoggerDialog::get()->Error("Send Data", "Sending data problems.");
    return false;
}

bool RoverClient::SendData(QString data)
{
    if(!IsConnected())
    {
        LoggerDialog::get()->Error("Send Data", "No connection");
        return false;
    }

    int cnt = _socket->write(data.toUtf8());
    if(cnt == data.count())
        return true;

    LoggerDialog::get()->Error("Send Data", "Sending data problems.");
    return false;
}

bool RoverClient::_startConnecting(QString _serviceName)
{
    if(_socket != nullptr)
    {
        if(_socket->SucessfulConnected())
            _socket->close();

        _socket->deleteLater();
    }

    _socket = nullptr;

    serviceName = _serviceName;
    return _infoClient->Refresh();
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
            onConnection();

            return;
        }
    }

    LoggerDialog::get()->Error("Connect to service","Can't find info about service: " + serviceName);
}

void RoverClient::on_socket_data()
{
    QByteArray data = _socket->readAll();
    Recorder::get()->WriteData(clientName(), data);
    onData(data);
}


