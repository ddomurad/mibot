#ifndef STATUSSTRATEGY_H
#define STATUSSTRATEGY_H

#include <QObject>
#include <mibAbstractSocketStrategy.h>

namespace mibot
{

class StatusStrategy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    explicit StatusStrategy(mibot::Connection *connection);
    ~StatusStrategy();

signals:

public slots:

    // AbstractSocketStrategy interface
protected:
    void processNewData(QByteArray);
    bool init();
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);

#endif // STATUSSTRATEGY_H
