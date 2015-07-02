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


Listener::Listener(SocketSetting *sock) :
    QObject(nullptr), _sockSettings(sock)
{
    if(_sockSettings->useSsl->value)
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
    if(_sockSettings != nullptr)
    {
        _sockSettings->Release();
        _sockSettings = nullptr;
    }
}

bool Listener::InitCertificates(QString caCrtDir, QString crt)
{
    if( _sockSettings->useSsl->value )
    {
        _caCrtDir = caCrtDir;
        _crtName = crt;
        if( !initCaCertificates() )
            return false;
    }
    else
    {
        LOG_WARNING( QString("Ssl support in socket '%1'' is disabled.").arg(_sockSettings->alias->value));
    }

    return true;
}


void Listener::StartListen()
{
    if(!_server->listen( QHostAddress::Any, _sockSettings->port->value ))
    {
        LOG_ERROR(QString("Listener '%1' starting error.").arg(_sockSettings->alias->value));
        emit ListenError();
    }
    else
    {
        LOG_INFO(QString("Listener '%1' is started.").arg(_sockSettings->alias->value));
        emit ListeningStarted();
    }
}

void Listener::StopListen()
{
    LOG_INFO(QString("Stoping listener '%1'.").arg(_sockSettings->alias->value));
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

        LOG_INFO(QString("Incomming connection from '%1'")
                 .arg(pendingSocket->peerAddress().toString()));


        pendingSocket->setSocketOption(QAbstractSocket::KeepAliveOption, true );
        if(_sockSettings->useSsl->value)
        {
            QSslSocket * sslPendingSocket = (QSslSocket *) pendingSocket;
            connect( sslPendingSocket, SIGNAL(sslErrors(QList<QSslError>)),
                     this, SLOT(onSslError(QList<QSslError>)) );

            connect(sslPendingSocket,SIGNAL(encrypted()),
                    this, SLOT(onConnectionEncrypted()));

            connect(sslPendingSocket, SIGNAL(peerVerifyError(QSslError)),
                    this, SLOT(onPeerVerifyError(QSslError)));

            sslPendingSocket->setCaCertificates( _certs );
            sslPendingSocket->setProtocol(QSsl::SecureProtocols);
            sslPendingSocket->setLocalCertificate( QString("%1/%2.crt").arg( _caCrtDir, _crtName) );
            sslPendingSocket->setPrivateKey( QString("%1/%2.key").arg( _caCrtDir, _crtName) );
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
    LOG_ERROR(QString("Accept error (%1: %2)")
              .arg(int(e)).arg( ((QTcpServer*)sender())->errorString() ));
}

void Listener::onSslError(QList<QSslError> e)
{
    //QTcpSocket * socket = (QTcpSocket*)sender();

    for( QSslError error : e )
    {
        LOG_ERROR(QString("Ssl error (%1).").arg(error.errorString()));
    }
}

void Listener::onConnectionEncrypted()
{
    QTcpSocket * socket = (QTcpSocket*)sender();
    QSslSocket * sslSocket = (QSslSocket*)sender();

    QList<QString> peerCommonNames = sslSocket->peerCertificate().subjectInfo(QSslCertificate::CommonName);

    if(peerCommonNames.count() != 0)
    {
        LOG_INFO(QString("Connection encrypted ('%1': '%2')")
                 .arg( socket->peerAddress().toString(),peerCommonNames[0] ));
    }else
    {
        LOG_INFO(QString("Connection encrypted ('%1')")
                 .arg( socket->peerAddress().toString()));
    }
}

void Listener::onPeerVerifyError(QSslError e)
{
    LOG_ERROR(QString("PeerVerifyError error ( %1 ).")
              .arg(e.errorString()) );
}

bool Listener::initCaCertificates()
{
    _certs.clear();
    if(_sockSettings == nullptr)
        return false;

    QStringList usersList = SettingsClient::GetResourceList(UserSetting::DefaultDir);

    if(usersList.empty())
    {
        LOG_ERROR( "Cant get users list." );
        return false;
    }

    for(QString userResName : usersList)
    {
        QString userResPath = UserSetting::DefaultDir + userResName;
        UserSetting * userObj = SettingsClient::CreateReource<UserSetting>(userResPath);

        if(!userObj->Sync())
        {
            LOG_ERROR(QString("Can't get user setting '%1'").arg(userResPath));
            continue;
        }

        if(userObj->isEnabled->value == false)
        {
            userObj->Release();
            continue;
        }

        if(userObj->certificate->value.isEmpty())
        {
            LOG_WARNING( QString("User: %1[res: %2] has no certificate specified").arg(userObj->alias->value, userObj->Resource()));
            userObj->Release();
            continue;
        }

        QString caCrtName = userObj->certificate->value;

        if(caCrtName.isEmpty() || caCrtName.contains('.'))
        {
            LOG_ERROR(QString("CaCertficate name can't be empnty or contains a '.' char. (%1)")
                      .arg(caCrtName));

            userObj->Release();
            continue;
        }

        QString caPath = QString("%1/%2.crt").arg( _caCrtDir, caCrtName );
        QFile crtFile( caPath );

        if( !crtFile.open( QIODevice::ReadOnly) )
        {
            LOG_ERROR(QString("Can't open certificate file (%1)")
                      .arg(caPath));

            userObj->Release();
            continue;
        }

        _certs.append( QSslCertificate( crtFile.readAll() ));
        crtFile.close();

        LOG_INFO( QString("CaCertificate laoded (%1) for (%2)")
                  .arg( caPath, _sockSettings->alias->value ) );

        userObj->Release();
    }

    return true;
}

void Listener::emitNewConnection(QTcpSocket * socket, bool ssl, QString errorString)
{
    Connection * connection = new Connection;

    connection->UseSsl = ssl;

    connection->TcpSocket   =   socket;
    connection->ErrorStrnig = errorString;
    connection->UserObj =   nullptr;
    connection->SocketObj = _sockSettings->CloneRef<SocketSetting>();
    connection->Status   =   errorString.isEmpty() ?
                Connection::Success :
                Connection::Failure;

    if(ssl && socket)
    {
        QSslSocket * sslSocket = (QSslSocket*) socket;
        QList<QString> peerIDs = sslSocket->peerCertificate().subjectInfo(QSslCertificate::CommonName);
        if(peerIDs.count() != 0)
        {
            UserSetting * userObj = SettingsClient::CreateReource<UserSetting>(peerIDs[0]);
            if(!userObj->Sync())
            {
                LOG_ERROR("Can't get user settings.");
                userObj->Release();
                connection->UserObj = nullptr;
            }else
            {
                connection->UserObj = userObj;
            }
        }
    }

    qDebug() << "New conenction ssl:" << ssl
             << "user:" << (connection->UserObj == nullptr ? "unknown" : connection->UserObj->Resource())
             << "error:" << connection->ErrorStrnig
             << "status:" << connection->Status;

    emit NewConnection( connection );
}

