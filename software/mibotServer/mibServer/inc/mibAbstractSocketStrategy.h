#ifndef ABSTRACTSOCKETSTRATEGY_H
#define ABSTRACTSOCKETSTRATEGY_H

#include <QObject>
#include <QLocalSocket>

#include "mibConnection.h"

namespace mibot
{

class Connection;
class AbstractSocketStrategy : public QObject
{
    Q_OBJECT
public:
    ~AbstractSocketStrategy();

    static bool ApplyStrategy(Connection*connection);
signals:

public slots:
    void ProcessData();

protected:
    explicit AbstractSocketStrategy(Connection * connection );
    Connection * _connection;
    virtual void processNewData(QByteArray ) = 0;
    virtual bool init() = 0;
};

class EchoStrategy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    explicit EchoStrategy(Connection * connection);
    ~EchoStrategy();


    // AbstractSocketStrategy interface
protected:
    void processNewData(QByteArray);
    bool init();

private:
    static int _cnt;
};


}
#endif // ABSTRACTSOCKETSTRATEGY_H
