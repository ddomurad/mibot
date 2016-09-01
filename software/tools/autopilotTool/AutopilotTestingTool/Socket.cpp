#include <cstdio>
#include "Socket.h"

Socket::Socket(QString connectionName, bool ssl, QObject *parent) :
    QSslSocket(parent),
    _connectionName(connectionName),
    _sucessfulConnected(false),
    _ssl(ssl)
{
    connect( this, SIGNAL(error(QAbstractSocket::SocketError)), (Socket*)this,SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect( this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    if(_ssl)
    {
        connect( this, SIGNAL(peerVerifyError(QSslError)), (Socket*)this, SLOT(onPeerVerifyError(QSslError)) );
        connect( this, SIGNAL(sslErrors(QList<QSslError>)), (Socket*)this, SLOT(onSslErrors(QList<QSslError>)));
    }
}

Socket::~Socket()
{}

bool Socket::ConnectToRover(QString host, int port)
{
    this->connectToHost(host, port);
    _sucessfulConnected = this->waitForConnected(5000);
    return _sucessfulConnected;
}

bool Socket::SucessfulConnected()
{
    return _sucessfulConnected;
}


void Socket::onSocketError(QAbstractSocket::SocketError)
{
    printf("%s", QString(_connectionName + " Socket error: " + ((QSslSocket*)sender())->errorString() +"\n").toStdString().c_str());
}

void Socket::onPeerVerifyError(QSslError err)
{
    printf("%s", QString(_connectionName + " PeerVerifyError: " + err.errorString() +"\n").toStdString().c_str());
}

void Socket::onSslErrors(QList<QSslError> errs)
{
    for(QSslError err : errs  )
        printf("%s", QString(_connectionName + " SslError: " + err.errorString() +"\n").toStdString().c_str());
}

void Socket::onDisconnected()
{
    _sucessfulConnected = false;
}

