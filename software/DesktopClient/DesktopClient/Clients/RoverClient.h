#ifndef ROVERCLIENT_H
#define ROVERCLIENT_H

#include "InfoServiceClient.h"
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
    virtual bool Connect() = 0;
signals:

private slots:
    void on_service_info();
    void on_socket_data();

protected:    
    bool _startConnecting(QString _serviceName);

    virtual void onData(QByteArray data) = 0;
    virtual void onConnection() = 0;

    Socket * _socket;
    InfoServiceClient* _infoClient;
};

#endif // ROVERCLIENT_H
