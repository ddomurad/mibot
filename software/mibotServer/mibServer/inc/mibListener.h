#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QtNetwork>

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
    explicit Listener(QString name, u_int16_t port,  bool ssl);
    ~Listener();

    void SetCertificates(QString localCrt, QString localKey, QString caCrt);
    bool UsingSsl();
    QString Name();

signals:
    void ListenError();
    void ListeningStarted();
    void ListeningStoped();
    void NewConnection(QTcpSocket *);

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
    QString _name;
    bool _use_ssl;
    u_int16_t _port;

    QString     _caCrt;
    QString     _localCrt;
    QString     _localKey;
};

}
#endif // LISTENER_H
