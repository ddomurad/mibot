#ifndef INFOSERVICECLIENT_H
#define INFOSERVICECLIENT_H

#include "Socket.h"
#include "mibJSONProtocol.h"
#include <QObject>

class ServiceInfo
{
public:
    QString name;
    QString type;
    int port;
    bool ssl;
    int accessLevel;
};

class InfoServiceClient : public QObject
{
    Q_OBJECT
public:
    InfoServiceClient(QObject * parent);
    ~InfoServiceClient();

    bool Refresh();
    QList<ServiceInfo> GetServiceInfos();

signals:
    void onRefreshDone();

private slots:
    void onSocketReadyRead();


private:
    Socket * _socket;

    mibot::JSONProtocol _jsonProtocol;

    void fixIfJsonIsCorrupted();
    void processObject(QJsonObject & object);

    bool _isRefreshed;

    QList<ServiceInfo> _infos;
};

#endif // INFOSERVICECLIENT_H
