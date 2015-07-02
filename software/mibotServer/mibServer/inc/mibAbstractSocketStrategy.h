#ifndef ABSTRACTSOCKETSTRATEGY_H
#define ABSTRACTSOCKETSTRATEGY_H

#include <QObject>
#include "mibConnection.h"

namespace mibot
{

class Connection;
class AbstractSocketStrategy : public QObject
{
    Q_OBJECT
public:
    ~AbstractSocketStrategy();

    static bool ApplyStrategy(Connection *connection);
signals:

public slots:
    void ProcessData();

protected:
    explicit AbstractSocketStrategy(Connection * connection );
    Connection * _connection;
    virtual void processNewData(QByteArray ) = 0;
    virtual bool init() = 0;
};

}
#endif // ABSTRACTSOCKETSTRATEGY_H
