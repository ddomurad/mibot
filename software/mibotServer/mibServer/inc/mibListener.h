#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QtNetwork>

#include "mibConnection.h"

namespace mibot
{

class SslTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit SslTcpServer(QObject *parent = nullptr);
    ~SslTcpServer();

protected:
    void incomingConnection(qintptr descriptor);
};

class Listener : public QObject
{
    Q_OBJECT
public:
    explicit Listener( SocketSetting *sock );
    ~Listener();

    bool InitCertificates(QString caCrtDir, QString crt);
    bool UsingSsl();
    QString Name();

signals:
    void ListenError();
    void ListeningStarted();
    void ListeningStoped();
    void NewConnection(Connection*);

public slots:
    void StartListen();
    void StopListen();

private slots:
    void onIncommingConnection();
    void onAcceptError(QAbstractSocket::SocketError);
    void onSslError(QList<QSslError>);
    void onConnectionEncrypted();
    void onPeerVerifyError(QSslError);

private:

    bool initCaCertificates();
    QTcpServer * _server;
    SocketSetting *_sockSettings;

    QString     _caCrtDir;
    QString     _crtName;
    QStringList _caCrts;

    QList<QSslCertificate> _certs;
    void emitNewConnection(QTcpSocket *, bool ssl, QString errorString);
};

}
#endif // LISTENER_H
