#include "inc/mibSslServer.h"
#include <QFile>

using namespace mibot;


SslServer::SslServer(QObject *parent) :
    QTcpServer(parent),
    _server_key(nullptr),
    _server_certificate(nullptr)
{}

SslServer::~SslServer()
{
    if(_server_key != nullptr)
        delete _server_key;

    if(_server_certificate != nullptr)
        delete _server_certificate;

    for(int i=0; i<_client_certificates.length(); i++)
        delete _client_certificates[i];
}

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

    _server_key = new QSslKey( keyFile.readAll(),
                      QSsl::Rsa, QSsl::Pem,QSsl::PrivateKey,
                      passphrase.toLatin1());

    if(_server_key->isNull())
    {
        delete _server_key;
        _server_key = nullptr;
        _errors.append( QString("Can't open given SslKey.") );
        return false;
    }

    return true;
}

bool SslServer::LoadServerCertificate(QString path)
{
    QSslCertificate * _cert = loadCertificate(path);

    if(_cert == nullptr) return false;

    _server_certificate = _cert;

    return true;
}

bool SslServer::AddClientCertificate(QString path)
{
    QSslCertificate * _cert = loadCertificate(path);

    if(_cert == nullptr) return false;

    _client_certificates.append( _cert );

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

QSslCertificate *SslServer::loadCertificate(QString path)
{
    QFile certFile(path);
    if(!certFile.open(QIODevice::ReadOnly))
    {
        _errors.append( QString("Can't open given SslCertificate.") );
        return nullptr;
    }

    QSslCertificate * _cert = new QSslCertificate( certFile.readAll(), QSsl::Pem );
    if(_cert->isNull())
    {
        delete _cert;
        _errors.append( QString("Can't open given SslCertificate.") );
        return nullptr;
    }

    return _cert;
}
