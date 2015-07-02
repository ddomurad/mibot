#ifndef CLIENTLISTENER_H
#define CLIENTLISTENER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSslSocket>

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

class ClientListener : public QObject
{
    Q_OBJECT
public:
    explicit ClientListener(QObject *parent = 0);
    ~ClientListener();

    bool startListen(QJsonObject &settings);
    void stopListen();
signals:
    void newClient(QTcpSocket * client);

private slots:
    void onIncommingConnection();
    void onAcceptError(QAbstractSocket::SocketError);
    void onSslError(QList<QSslError> e);
    void onConnectionEncrypted();
    void onPeerVerifyError(QSslError);

private:
    SslTcpServer * _server;

    QString _certFile;
    QString _caCertFile;
    QList<QSslCertificate> _caCerts;

    bool checkSetting(QString name, QJsonObject &obj);

};
}
#endif // CLIENTLISTENER_H
