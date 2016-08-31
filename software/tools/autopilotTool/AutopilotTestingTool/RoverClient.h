#ifndef ROVERCLIENT_H
#define ROVERCLIENT_H

#include "Socket.h"
#include <QObject>
#include <QWidget>

class RoverClient : public QObject
{
    Q_OBJECT
public:
    explicit RoverClient(QObject *parent = 0);
    ~RoverClient();

    QString serviceName;

    bool IsConnected();
    void Disconnect();
    bool SendData(QString data);
    bool SendData(uchar * data, int size);
    virtual bool Connect(QString addr, int port) = 0;
    virtual void onData(QByteArray data) = 0;
signals:

private slots:
    void on_socket_data();

protected:    
    bool _startConnecting(QString _serviceName, int port);
    virtual void onConnection() = 0;
    virtual QString clientName() = 0;

    Socket * _socket;
};

#endif // ROVERCLIENT_H
