#include "RoverClient.h"

RoverClient::RoverClient(QObject *parent) :
    QObject(parent),
    _socket(nullptr)
{}

RoverClient::~RoverClient()
{
    if(_socket != nullptr)
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
        printf("Send Data: No connection\n");
        return false;
    }

    int cnt = _socket->write((char*)data, size);
    if(cnt == size)
        return true;

    printf("Send Data: Sending data problems.\n");
    return false;
}

bool RoverClient::SendData(QString data)
{
    if(!IsConnected())
    {
        printf("Send Data: No connection\n");
        return false;
    }

    int cnt = _socket->write(data.toUtf8());
    if(cnt == data.count())
        return true;

    printf("Send Data: Sending data problems.\n");
    return false;
}

bool RoverClient::_startConnecting(QString addr, int port)
{
    _socket = new Socket(serviceName,false, this);

    connect(_socket, SIGNAL(readyRead()), this, SLOT(on_socket_data()));

    if(!_socket->ConnectToRover(addr, port))
    {
        printf("Connect to service: Can't connect to: %s\n", serviceName.toStdString().c_str());
        return false;
    }

    printf("Connect to service: Connected to %s\n", serviceName.toStdString().c_str());
    onConnection();

    return true;
}


void RoverClient::on_socket_data()
{
    QByteArray data = _socket->readAll();
    onData(data);
}


