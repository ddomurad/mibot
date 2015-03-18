#include "inc/mibSslServer.h"
#include <QFile>

using namespace mibot;


SslPoweredTcpServer::SslPoweredTcpServer(QObject *parent) :
    QTcpServer(parent)
{}

SslPoweredTcpServer::~SslPoweredTcpServer()
{}

void SslPoweredTcpServer::incomingConnection(qintptr descriptor)
{
    QSslSocket *in_socket = new QSslSocket(this);
    in_socket->setSocketDescriptor( descriptor );

    this->addPendingConnection( in_socket );
}



SslServer::SslServer(QObject *parent) :
    QThread(parent),
    _server_socket(nullptr)
{}

SslServer::~SslServer()
{}

bool SslServer::LoadServerKeyFile(QString path)
{
    return LoadServerKeyFile( path, QString() );
}

bool SslServer::LoadServerKeyFile(QString path, QString passphrase)
{
    QFile keyFile(path);
    if(!keyFile.open(QIODevice::ReadOnly))
    {
        _errors.append( QString("Can't open given SslKey.") );
        return false;
    }

    _server_key = QSslKey( keyFile.readAll(),
                           QSsl::Rsa, QSsl::Pem,QSsl::PrivateKey,
                           passphrase.toLatin1());

    if(_server_key.isNull())
    {
        _errors.append( QString("Can't open given SslKey.") );
        return false;
    }

    return true;
}

bool SslServer::LoadServerCertificate(QString path)
{
    if(!loadCertificate(path, &_server_certificate)) return false;

    return true;
}

bool SslServer::AddClientCertificate(QString path)
{
    QSslCertificate cert;
    if(!loadCertificate(path,&cert)) return false;

    _client_certificates.append( cert );

    return true;
}

CertificateInfo SslServer::ServerCertificateInfo()
{
    return CertificateInfo( _server_certificate );
}

CertificateInfo SslServer::ClientCertificateInfo(int num)
{
    return CertificateInfo(_client_certificates[num]);
}

int SslServer::ClientCertificateCount()
{
    return _client_certificates.length();
}

QString SslServer::NextError()
{
    if(_errors.empty()) return "";
    return _errors.dequeue();
}

bool SslServer::AreErrors()
{
    return !_errors.empty();
}

void SslServer::run()
{
    _server_socket = new SslPoweredTcpServer();

    connect( _server_socket, SIGNAL(newConnection()), this, SLOT(onIncomingConnection()), Qt::DirectConnection );
    connect( _server_socket, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(onAcceptError(QAbstractSocket::SocketError)), Qt::DirectConnection );

    if( !_server_socket->listen( _listeningAddr,  _listeningPort ) )
    {
        exit(1);
        _errors.enqueue( "Server listening error." );
    }

    exec();

    _server_socket->close();
    delete _server_socket;
    _server_socket = nullptr;
}

void SslServer::StartServer(QHostAddress addr, u_int16_t port)
{
    if(isRunning())
    {
        _errors.enqueue( "Server alreading started." );
        return;
    }

    _listeningAddr = addr;
    _listeningPort = port;

    start();
    sleep(2);
}

void SslServer::StopServer()
{
    if(!isRunning())
    {
        _errors.enqueue( "Server alreading stoped." );
        return;
    }

    quit();
    if( !wait( 1000 ) )
    {
        _errors.enqueue( "Thread joining error." );
    }
}

void SslServer::onSslErrors(QList<QSslError> errors)
{
    if(errors.length() == 0) return;

    QString strb = "SSL ERRORS [";

    for(int i=0; i<errors.length(); i++)
    {
        strb += sslErrorToString( errors[i].error() );
        if(i != errors.length() - 1) strb += "; ";
    }
    strb  += "]";

    _errors.append( strb );
}

void SslServer::onAcceptError(QAbstractSocket::SocketError error)
{
    _errors.append( QString("Socket error (%1).").arg(error) );
}

void SslServer::onIncomingConnection()
{
    QSslSocket * in_socket = (QSslSocket *)_server_socket->nextPendingConnection();

    in_socket->setCaCertificates( _client_certificates );
    in_socket->setSocketOption( QAbstractSocket::KeepAliveOption, true );
    in_socket->setProtocol( QSsl::SslV3 );
    in_socket->setLocalCertificate( _server_certificate );
    in_socket->setPrivateKey( _server_key );

    connect( in_socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)) );
    connect(in_socket,SIGNAL(encrypted()),this,SLOT(onClientEncrypted()));

    in_socket->startServerEncryption();
}

void SslServer::onClientEncrypted()
{

}

bool SslServer::loadCertificate(QString path, QSslCertificate * out_cert)
{
    QFile certFile(path);
    if(!certFile.open(QIODevice::ReadOnly))
    {
        _errors.append( QString("Can't open given SslCertificate.") );
        return false;
    }

    QSslCertificate cert = QSslCertificate( certFile.readAll(), QSsl::Pem );
    if(cert.isNull())
    {
        _errors.append( QString("Can't open given SslCertificate.") );
        return false;
    }

    *out_cert = cert;

    return true;
}

QString SslServer::sslErrorToString(QSslError::SslError e)
{
    switch(e)
    {
    case QSslError::NoError: return "NoError";
    case QSslError::UnableToGetIssuerCertificate: return "UnableToGetIssuerCertificate";
    case QSslError::UnableToDecryptCertificateSignature: return "UnableToDecryptCertificateSignature";
    case QSslError::UnableToDecodeIssuerPublicKey: return "UnableToDecodeIssuerPublicKey";
    case QSslError::CertificateSignatureFailed: return "CertificateSignatureFailed";
    case QSslError::CertificateNotYetValid: return "CertificateNotYetValid";
    case QSslError::CertificateExpired: return "CertificateExpired";
    case QSslError::InvalidNotBeforeField: return "InvalidNotBeforeField";
    case QSslError::InvalidNotAfterField: return "InvalidNotAfterField";
    case QSslError::SelfSignedCertificate: return "SelfSignedCertificate";
    case QSslError::SelfSignedCertificateInChain: return "SelfSignedCertificateInChain";
    case QSslError::UnableToGetLocalIssuerCertificate: return "UnableToGetLocalIssuerCertificate";
    case QSslError::UnableToVerifyFirstCertificate: return "UnableToVerifyFirstCertificate";
    case QSslError::CertificateRevoked: return "CertificateRevoked";
    case QSslError::InvalidCaCertificate: return "InvalidCaCertificate";
    case QSslError::PathLengthExceeded: return "PathLengthExceeded";
    case QSslError::InvalidPurpose: return "InvalidPurpose";
    case QSslError::CertificateUntrusted: return "CertificateUntrusted";
    case QSslError::CertificateRejected: return "CertificateRejected";
    case QSslError::SubjectIssuerMismatch: return "SubjectIssuerMismatch";
    case QSslError::AuthorityIssuerSerialNumberMismatch: return "AuthorityIssuerSerialNumberMismatch";
    case QSslError::NoPeerCertificate: return "NoPeerCertificate";
    case QSslError::HostNameMismatch: return "HostNameMismatch";
    case QSslError::NoSslSupport: return "NoSslSupport";
    case QSslError::CertificateBlacklisted: return "CertificateBlacklisted";
    case QSslError::UnspecifiedError: return "UnspecifiedError";
    }
    return "UnspecifiedError";
}

