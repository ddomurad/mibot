#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QtNetwork>
#include <QSslKey>
#include "mibCertificateInfo.h"

namespace mibot
{

class SslServer : public QTcpServer
{
    Q_OBJECT
public:
    SslServer(QObject *parent = nullptr);
    ~SslServer();

    bool LoadServerKeyFile(QString path);
    bool LoadServerKeyFile(QString path, QString passphrase);
    bool LoadServerCertificate(QString path);

    bool AddClientCertificate(QString path);

    CertificateInfo ServerCertificateInfo();
    CertificateInfo ClientCertificateInfo(int num);
    int ClientCertificateCount();

    QString NextError();

private:
    QSslKey * _server_key;
    QSslCertificate * _server_certificate;

    QList< QSslCertificate* > _client_certificates;
    QQueue< QString >   _errors;

    QSslCertificate* loadCertificate(QString path);
};

}
#endif // SSLSERVER_H
