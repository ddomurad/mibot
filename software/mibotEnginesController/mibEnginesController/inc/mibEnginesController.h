#ifndef MIBENGINESCONTROLLER_H
#define MIBENGINESCONTROLLER_H

#include <QObject>
#include <QJsonObject>
#include <QtNetwork>
#include <mibGlobalAccess.h>

#include "mibEnginesControllerGlobal.h"
#include "mibControllerStatus.h"
#include "mibMotorController.h"
#include "mibGPIO.h"

namespace mibot
{
class MIBENGINESCONTROLLERSHARED_EXPORT ControllerObject :
        public QObject
{
    Q_OBJECT
public:
    ControllerObject(QObject * parent, uint emergency_break);
    ~ControllerObject();

public slots:
    void onUpdate(float);
    void onRead();

public:
    QLocalSocket *  socket;
    ControllerStatusWrapper * status;

    MotorController * controllers[2];

    MappedGPIO * fakeGPIO;

    static MappedGPIO * realGPIO;
private:
    MotorController * last_controllers;
    QElapsedTimer emergency_break_timer;
    uint emergency_break_timeout;
};

class MIBENGINESCONTROLLERSHARED_EXPORT EnginesController :
        public QObject
{
    Q_OBJECT
public:
    EnginesController(QObject * parent);
    ~EnginesController();

    bool Init();

private slots:
    void onConnection();
    void onDisconnection();
    void onError(QLocalSocket::LocalSocketError);
    void onUpdate();
private:
    QString getConfig(QString key);
    int getConnectionIndex(QLocalSocket * socket);

    QLocalServer * _local_server;
    ResourcesSet<GlobalConfigRes> * _global_config;

    QList<ControllerObject*> _connections;
    QTimer  *_update_timer;

    QElapsedTimer _elapsed_timer;
    uint          emergency_break_timeout;
};

}
#endif // MMIBENGINESCONTROLLER_H
