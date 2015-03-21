#ifndef MIBSERVER_H
#define MIBSERVER_H

#include "mibserver_global.h"

#include <QObject>
#include <QJsonObject>

#include "mibListener.h"
#include "mibConnectionHandler.h"

namespace mibot
{

class MIBSERVERSHARED_EXPORT Server : public QObject
{
    Q_OBJECT
public:
    ~Server();

    static Server *BuildServer(QJsonObject &, QObject *parent);

    void AddListener( Listener *);

signals:
    void ServerStarted();
    void ServerStoped();

public slots:
    void StartServer();
    void StopServer();

private:
    explicit Server(QObject * parent);

    ConnectionHandler * _connectionHandler;

    QList<Listener*> _listeners;
    QThread * _listenersThread;
};

}

#endif // MIBSERVER_H
