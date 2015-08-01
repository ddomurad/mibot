#include "inc/mibClientListener.h"
#include <mibLogger.h>

#include <QJsonObject>
#include <QHostAddress>

using namespace mibot;

SslTcpServer::SslTcpServer(QObject *parent) :
    QTcpServer(parent)
{}

SslTcpServer::~SslTcpServer()
{}

void SslTcpServer::incomingConnection(qintptr descriptor)
{
    QSslSocket *in_socket = new QSslSocket( this );
    in_socket->setSocketDescriptor( descriptor );

    this->addPendingConnection( in_socket );
}



ClientListener::ClientListener(QObject *parent) :
    QObject(parent),
    _server(nullptr)
{}

ClientListener::~ClientListener()
{    
}

bool ClientListener::startListen(QJsonObject &settings)
{
    if(_server != nullptr)
        _server->deleteLater();

    if(!checkSetting("Port",settings)
            || !checkSetting("Cert",settings)
            || !checkSetting("CaCert",settings))
        return false;

    int port = settings["Port"].toInt();
    _certFile = settings["Cert"].toString();
    _caCertFile = settings["CaCert"].toString();

    _caCerts.clear();
    _caCerts.append(QSslCertificate::fromPath(_caCertFile));
    if(_caCerts.count() == 0)
    {
        LOG_ERROR(QString("Can't load certificates from file: \"%1\"").arg(_caCertFile));
        return false;
    }    
    _server = new SslTcpServer(this);

    connect( _server, SIGNAL(acceptError(QAbstractSocket::SocketError)),
             this, SLOT(onAcceptError(QAbstractSocket::SocketError)));

    connect( _server, SIGNAL(newConnection()),
             this, SLOT(onIncommingConnection()));

    if(!_server->listen(QHostAddress::Any, port))
    {
        LOG_ERROR("Server can't listen: " + _server->errorString());
        return false;
    }

    LOG_INFO( QString("ClientListener started on port: %1").arg(port) );
    return true;
}

void ClientListener::stopListen()
{
    if(_server == nullptr) return;
    _server->close();
    _server->deleteLater();
    _server = nullptr;
}

void ClientListener::onIncommingConnection()
{
    while(_server->hasPendingConnections())
    {
        QTcpSocket * pendingSocket = _server->nextPendingConnection();
        QSslSocket * sslPendingSocket = (QSslSocket *) pendingSocket;

        LOG_INFO(QString("Incomming connection from '%1'")
                 .arg(sslPendingSocket->peerAddress().toString()));

        sslPendingSocket->setSocketOption(QAbstractSocket::KeepAliveOption, true );


        connect( sslPendingSocket, SIGNAL(sslErrors(QList<QSslError>)),
                 this, SLOT(onSslError(QList<QSslError>)) );

        connect(sslPendingSocket,SIGNAL(encrypted()),
                this, SLOT(onConnectionEncrypted()));

        connect(sslPendingSocket, SIGNAL(peerVerifyError(QSslError)),
                this, SLOT(onPeerVerifyError(QSslError)));


        sslPendingSocket->setCaCertificates( _caCerts );
        sslPendingSocket->setProtocol(QSsl::SecureProtocols);
        sslPendingSocket->setLocalCertificate( QString("%1.crt").arg( _certFile) );
        sslPendingSocket->setPrivateKey( QString("%1.key").arg( _certFile) );
        sslPendingSocket->setPeerVerifyMode(QSslSocket::VerifyPeer);

        sslPendingSocket->startServerEncryption();

        if( !sslPendingSocket->waitForEncrypted( 1000 ))
        {
            LOG_WARNING(QString("Encryption failure (%1)").arg(sslPendingSocket->peerAddress().toString()));
            sslPendingSocket->disconnectFromHost();
        }
        else
        {
            emit newClient(pendingSocket);
        }
    }
}

void ClientListener::onAcceptError(QAbstractSocket::SocketError e)
{
    LOG_ERROR(QString("Accept error (%1: %2)")
              .arg(int(e)).arg( ((QTcpServer*)sender())->errorString() ));
}

void ClientListener::onSslError(QList<QSslError> e)
{
    for( QSslError error : e )
    {
        LOG_ERROR(QString("Ssl error (%1).").arg(error.errorString()));
    }
}

void ClientListener::onConnectionEncrypted()
{
    QTcpSocket * socket = (QTcpSocket*)sender();
    QSslSocket * sslSocket = (QSslSocket*)sender();

    QList<QString> peerCommonNames = sslSocket->peerCertificate().subjectInfo(QSslCertificate::CommonName);

    if(peerCommonNames.count() != 0)
    {
        LOG_INFO(QString("Connection encrypted ('%1': '%2')")
                 .arg( socket->peerAddress().toString(), peerCommonNames[0] ));
    }else
    {
        LOG_INFO(QString("Connection encrypted ('%1')")
                 .arg( socket->peerAddress().toString() ));
    }
}

void ClientListener::onPeerVerifyError(QSslError e)
{
    LOG_ERROR(QString("PeerVerifyError error ( %1 ).")
              .arg(e.errorString()) );
}

bool ClientListener::checkSetting(QString name, QJsonObject &obj)
{
    if(obj[name].isUndefined())
    {
        LOG_ERROR( QString("%1 setting is undefined.").arg(name) );
        return false;
    }

    return true;
}
