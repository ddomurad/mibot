#include "inc/mibListener.h"
#include <mibLogger.h>

using namespace mibot;

SslTcpServer::SslTcpServer(QObject *parent) :
    QTcpServer(parent)
{}

SslTcpServer::~SslTcpServer()
{}

void SslTcpServer::incomingConnection(qintptr descriptor)
{
    QSslSocket *in_socket = new QSslSocket(this);
    in_socket->setSocketDescriptor( descriptor );

    this->addPendingConnection( in_socket );
}


Listener::Listener(QString name, u_int16_t port, bool ssl) :
    QObject(nullptr),
    _name(name), _use_ssl(ssl), _port(port)
{
    if(_use_ssl)
    {
        _server = new SslTcpServer(this);
    }
    else
    {
        _server = new QTcpServer(this);

    }

    connect( _server, SIGNAL(acceptError(QAbstractSocket::SocketError)),
             this, SLOT(onAcceptError(QAbstractSocket::SocketError)));

    connect( _server, SIGNAL(newConnection()),
             this, SLOT(onIncommingConnection()));

}

Listener::~Listener()
{}

void Listener::SetCertificates(QString localCrt, QString localKey, QString caCrt)
{
    if(_use_ssl)
    {
        _localCrt = localCrt;
        _localKey = localKey;
        _caCrt = caCrt;
    }
    else
    {
        DEFLOG_ERROR("Can't set certificates to listener with disabled ssl support.");
    }
}

bool Listener::UsingSsl()
{
    return _use_ssl;
}

QString Listener::Name()
{
    return _name;
}

void Listener::StartListen()
{
    if(!_server->listen( QHostAddress::Any, _port ))
    {
        DEFLOG_ERROR(QString("Listener '%1' starting error.").arg(_name));
        emit ListenError();
    }
    else
    {
        DEFLOG_INFO(QString("Listener '%1' is started.").arg(_name));
        emit ListeningStarted();
    }
}

void Listener::StopListen()
{
    DEFLOG_INFO(QString("Stoping listener '%1'.").arg(_name));
    if(_server->isListening())
    {
        _server->close();
        emit ListeningStoped();
    }
}

void Listener::onIncommingConnection()
{
    while(_server->hasPendingConnections())
    {
        QTcpSocket * pendingSocket = _server->nextPendingConnection();

        LOG_INFO("audit", QString("Incomming connection from '%1'")
                 .arg(pendingSocket->peerAddress().toString()));

        if(_use_ssl)
        {
            QSslSocket * sslPendingsocket = (QSslSocket *) pendingSocket;
            connect( sslPendingsocket, SIGNAL(sslErrors(QList<QSslError>)),
                     this, SLOT(onSslError(QList<QSslError>)) );

            connect(sslPendingsocket,SIGNAL(encrypted()),
                    this, SLOT(onConnectionEncrypted()));

            connect(sslPendingsocket, SIGNAL(peerVerifyError(QSslError)),
                    this, SLOT(onPeerVerifyError(QSslError)));

            sslPendingsocket->setCaCertificates( QSslCertificate::fromPath(_caCrt) );
            sslPendingsocket->setSocketOption(QAbstractSocket::KeepAliveOption, true );
            sslPendingsocket->setProtocol(QSsl::SslV3);
            sslPendingsocket->setLocalCertificate(_localCrt);
            sslPendingsocket->setPrivateKey(_localKey);

            sslPendingsocket->startServerEncryption();
        }
        else
        {
            emit NewConnection( pendingSocket );
        }

    }
}

void Listener::onAcceptError(QAbstractSocket::SocketError e)
{
    LOG_ERROR("audit", QString("Accept error (%1: %2)")
              .arg(int(e)).arg( ((QTcpServer*)sender())->errorString() ));
}

void Listener::onSslError(QList<QSslError> e)
{
    QTcpSocket * socket = (QTcpSocket*)sender();

    for( QSslError error : e )
    {
        LOG_ERROR("audit", QString("Ssl error (%1).").arg(error.errorString()));
    }

    socket->close();
}

void Listener::onConnectionEncrypted()
{
    QTcpSocket * socket = (QTcpSocket*)sender();
    QSslSocket * sslSocket = (QSslSocket*)sender();

    QList<QString> peerCommonNames = sslSocket->peerCertificate().subjectInfo(QSslCertificate::CommonName);

    if(peerCommonNames.count() != 0)
    {
        LOG_INFO("audit", QString("Connection encrypted ('%1': '%2')")
             .arg( socket->peerAddress().toString(),peerCommonNames[0] ));
    }else
    {
        LOG_INFO("audit", QString("Connection encrypted ('%1')")
             .arg( socket->peerAddress().toString()));
    }

    emit NewConnection( socket );
}

void Listener::onPeerVerifyError(QSslError e)
{
    QTcpSocket * socket = (QTcpSocket*)sender();
    LOG_ERROR("audit", QString("PeerVerifyError error ( %1 ).")
              .arg(e.errorString()) );

    socket->close();
}

