#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QtNetwork>

#include <mibGlobalAccess.h>
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
    explicit Listener(SocketRes *sock, bool ssl);
    ~Listener();

    void SetCertificates(QString localCrt, QString localKey, QString caCrt);
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

    QTcpServer * _server;
    SocketRes *_sockRes;
    bool _use_ssl;

    QString     _caCrt;
    QString     _localCrt;
    QString     _localKey;

    void emitNewConnection(QTcpSocket *, bool ssl, QString errorString);
};

}
#endif // LISTENER_H
