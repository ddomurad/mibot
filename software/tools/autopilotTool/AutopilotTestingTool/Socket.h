#ifndef _SOCKET_H
#define _SOCKET_H

#include <QSslError>
#include <QtNetwork>

class Socket : public QSslSocket
{
    Q_OBJECT
public:
    Socket(QString connectionName, bool ssl, QObject * parent);
    ~Socket();

    bool ConnectToRover(QString host, int port);
    bool SucessfulConnected();

private slots:
    void onSocketError(QAbstractSocket::SocketError);
    void onPeerVerifyError(QSslError);
    void onSslErrors(QList<QSslError>);
    void onDisconnected();
private:
    QString _connectionName;
    bool _sucessfulConnected;
    bool _ssl;
};

#endif // SOCKET

