#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QtNetwork>
#include <QSslKey>
#include "mibCertificateInfo.h"

namespace mibot
{

class SslPoweredTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit SslPoweredTcpServer(QObject *parent = nullptr);
    ~SslPoweredTcpServer();

protected:
    void incomingConnection(qintptr descriptor);
};

class SslServer : public QThread
{
    Q_OBJECT
public:
    explicit SslServer(QObject *parent = nullptr);
    ~SslServer();

    bool LoadServerKeyFile(QString path);
    bool LoadServerKeyFile(QString path, QString passphrase);
    bool LoadServerCertificate(QString path);

    bool AddClientCertificate(QString path);

    CertificateInfo ServerCertificateInfo();
    CertificateInfo ClientCertificateInfo(int num);
    int ClientCertificateCount();

    QString NextError();
    bool AreErrors();

    void run();

public slots:
    void StartServer(QHostAddress addr, u_int16_t port);
    void StopServer();

private slots:
    void onSslErrors(QList<QSslError> errors);
    void onAcceptError(QAbstractSocket::SocketError error);
    void onIncomingConnection();
    void onClientEncrypted();

private:
    SslPoweredTcpServer *_server_socket;

    QSslKey _server_key;
    QSslCertificate _server_certificate;

    QList< QSslCertificate > _client_certificates;
    QQueue< QString >   _errors;

    bool loadCertificate(QString path, QSslCertificate *out_cert);
    QString sslErrorToString(QSslError::SslError error);

    QHostAddress _listeningAddr;
    u_int16_t   _listeningPort;
};

}
#endif // SSLSERVER_H
