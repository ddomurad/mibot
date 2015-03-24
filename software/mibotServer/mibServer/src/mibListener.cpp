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


Listener::Listener(SocketRes *sock) :
    QObject(nullptr), _sockRes(sock)
{
    if(_sockRes->UseSsl())
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

bool Listener::InitCertificates(QString crtDir, QString crt)
{
    if( _sockRes->UseSsl() )
    {
        _crtDir = crtDir;
        _crtName = crt;
        if( !initCaCertificates() )
            return false;
    }
    else
    {
        DEFLOG_WARNING("Ssl support is disabled.");
    }

    return true;
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
        if(_sockRes->UseSsl())
        {
            QSslSocket * sslPendingSocket = (QSslSocket *) pendingSocket;
            connect( sslPendingSocket, SIGNAL(sslErrors(QList<QSslError>)),
                     this, SLOT(onSslError(QList<QSslError>)) );

            connect(sslPendingSocket,SIGNAL(encrypted()),
                    this, SLOT(onConnectionEncrypted()));

            connect(sslPendingSocket, SIGNAL(peerVerifyError(QSslError)),
                    this, SLOT(onPeerVerifyError(QSslError)));

            sslPendingSocket->setCaCertificates( _certs );
            sslPendingSocket->setProtocol(QSsl::SslV3);
            sslPendingSocket->setLocalCertificate( QString("%1/%2.crt").arg( _crtDir, _crtName) );
            sslPendingSocket->setPrivateKey( QString("%1/%2.key").arg( _crtDir, _crtName) );
            sslPendingSocket->setPeerVerifyMode(QSslSocket::VerifyPeer);

            sslPendingSocket->startServerEncryption();

            if( !sslPendingSocket->waitForEncrypted( 1000 ))
            {
                sslPendingSocket->disconnectFromHost();
                emitNewConnection( sslPendingSocket, true , "Encryption failure." );
            }
            else
            {
                emitNewConnection( sslPendingSocket, true , QString());
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

bool Listener::initCaCertificates()
{
    _certs.clear();
    if(_sockRes == nullptr)
        return false;

    if(_certs.isEmpty())
    {
        ResourcesSet<CertificateSocketBoundRes> * set =
            GlobalAccess::CertificateSocketBoundsSesBySocket( _sockRes->Id() );

        if(set == nullptr)
        {
            DEFLOG_ERROR( "Certificates Set loading error." );
            return false;
        }

        for( int i=0; i<set->Count(); i++)
        {
            UsersCertificateRes * ucr =  GlobalAccess::UserCertificate( set->At(i)->Certificate() );
             if(ucr == nullptr)
             {
                 DEFLOG_ERROR(QString("Could not read UserCertificate from reposiroty: (Id:%1) ")
                              .arg(set->At(i)->Certificate().toString()));
                 continue;
             }

            QString caCrtName = ucr->FileName();
            if(caCrtName.isEmpty() || caCrtName.contains('.'))
            {
                DEFLOG_ERROR(QString("CaCertficate name can't be empnty or contains a '.' char. (%1)")
                             .arg(caCrtName));
                continue;
            }

            QString caPath = QString("%1/%2.crt").arg( _crtDir, caCrtName );
            QFile crtFile( caPath );

            if( !crtFile.open( QIODevice::ReadOnly) )
            {
                DEFLOG_ERROR(QString("Can't open certificate file (%1)")
                             .arg(caPath));
                continue;
            }

            _certs.append( QSslCertificate( crtFile.readAll() ));
            crtFile.close();

            DEFLOG_INFO( QString("CaCertificate laoded (%1) for (%2)")
                         .arg( caPath, _sockRes->Alias() ) );
        }

        delete set;
    }

    return true;
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

