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
    QSslSocket *in_socket = new QSslSocket( this );
    in_socket->setSocketDescriptor( descriptor );

    this->addPendingConnection( in_socket );
}


Listener::Listener(SocketRes *sock, bool ssl) :
    QObject(nullptr), _sockRes(sock), _use_ssl(ssl)
{
    if(_use_ssl)
        _server = new SslTcpServer(this);
    else
        _server = new QTcpServer(this);


    connect( _server, SIGNAL(acceptError(QAbstractSocket::SocketError)),
             this, SLOT(onAcceptError(QAbstractSocket::SocketError)));

    connect( _server, SIGNAL(newConnection()),
             this, SLOT(onIncommingConnection()));

}

Listener::~Listener()
{
    if(_sockRes != nullptr)
        delete _sockRes;
}

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


void Listener::StartListen()
{
    if(!_server->listen( QHostAddress::Any, _sockRes->Port() ))
    {
        DEFLOG_ERROR(QString("Listener '%1' starting error.").arg(_sockRes->Alias()));
        emit ListenError();
    }
    else
    {
        DEFLOG_INFO(QString("Listener '%1' is started.").arg(_sockRes->Alias()));
        emit ListeningStarted();
    }
}

void Listener::StopListen()
{
    DEFLOG_INFO(QString("Stoping listener '%1'.").arg(_sockRes->Alias()));
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


        pendingSocket->setSocketOption(QAbstractSocket::KeepAliveOption, true );
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
            sslPendingsocket->setProtocol(QSsl::SslV3);
            sslPendingsocket->setLocalCertificate(_localCrt);
            sslPendingsocket->setPrivateKey(_localKey);
            sslPendingsocket->setPeerVerifyMode(QSslSocket::VerifyPeer);

            sslPendingsocket->startServerEncryption();

            if( !sslPendingsocket->waitForEncrypted( 1000 ))
            {
                sslPendingsocket->disconnectFromHost();
                emitNewConnection( sslPendingsocket, true , "Encryption failure." );
            }
            else
            {
                emitNewConnection( sslPendingsocket, true , QString());
            }
        }
        else
        {
            emitNewConnection( pendingSocket, false, QString());
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
    //QTcpSocket * socket = (QTcpSocket*)sender();

    for( QSslError error : e )
    {
        LOG_ERROR("audit", QString("Ssl error (%1).").arg(error.errorString()));
    }
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
}

void Listener::onPeerVerifyError(QSslError e)
{
    //QTcpSocket * socket = (QTcpSocket*)sender();
    LOG_ERROR("audit", QString("PeerVerifyError error ( %1 ).")
              .arg(e.errorString()) );
}

void Listener::emitNewConnection(QTcpSocket * socket, bool ssl, QString errorString)
{
    Connection * connection = new Connection;

    connection->UseSsl = ssl;

    connection->Id       =   QUuid::createUuid();
    connection->Socket   =   _sockRes->Id();
    connection->TcpSocket   =   socket;
    connection->User     =   QUuid();
    connection->ErrorStrnig = errorString;
    connection->Status   =   errorString.isEmpty() ?
                            Connection::Success :
                            Connection::Failure;

    if(ssl && socket)
    {
        QSslSocket * sslSocket = (QSslSocket*) socket;
        QList<QString> peerIDs = sslSocket->peerCertificate().subjectInfo(QSslCertificate::CommonName);
        if(peerIDs.count() != 0) connection->User = QUuid( peerIDs[0] );
    }

    qDebug() << "New conenction ssl:" << ssl
             << "did:" << _sockRes->Id()
             << "user:" << connection->User
             << "error:" << connection->ErrorStrnig
             << "status:" << connection->Status;

    emit NewConnection( connection );
}

