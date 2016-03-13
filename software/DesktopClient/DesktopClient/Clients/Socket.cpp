#include "./Forms/LoggerDialog.h"
#include "./Clients/GlobalPasswordProvider.h"
#include "./Settings/AppSettings.h"
#include "./Clients/Socket.h"

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

bool Socket::ConnectToRover(int port)
{
   if(_ssl)
        {
            this->setLocalCertificate( AppSettings::GetKey(AppSettings::Connection_Cert_Path).toString() );
            this->setPrivateKey( AppSettings::GetKey(AppSettings::Connection_PK_Path).toString(),
                                      QSsl::Rsa, QSsl::Pem, GlobalPasswordProvider::Get().toLatin1());

            this->setProtocol(QSsl::SecureProtocols) ;

            this->setCaCertificates(QSslCertificate::fromPath(AppSettings::GetKey(AppSettings::Connection_CA_Path).toString()));

            this->connectToHostEncrypted(
                        AppSettings::GetKey(AppSettings::Connection_Rover_Addr).toString(),
                        port, "mibot_server");
        }else
        {
            this->connectToHost(AppSettings::GetKey(AppSettings::Connection_Rover_Addr).toString(),
                                     AppSettings::GetKey(AppSettings::Connection_Rover_Info_Port).toInt());
        }

    _sucessfulConnected = this->waitForConnected(5000);
    if(!_ssl)
        return _sucessfulConnected;

    return _sucessfulConnected = this->waitForEncrypted(5000);
}

bool Socket::SucessfulConnected()
{
    return _sucessfulConnected;
}


void Socket::onSocketError(QAbstractSocket::SocketError)
{
    LoggerDialog::get()->Error(_connectionName + " Socket error" , ((QSslSocket*)sender())->errorString());
}

void Socket::onPeerVerifyError(QSslError err)
{
    LoggerDialog::get()->Error(_connectionName + " PeerVerifyError" , err.errorString());
}

void Socket::onSslErrors(QList<QSslError> errs)
{
    for(QSslError err : errs  )
        LoggerDialog::get()->Error(_connectionName + " SslError" , err.errorString());
}

void Socket::onDisconnected()
{
    _sucessfulConnected = false;
}

