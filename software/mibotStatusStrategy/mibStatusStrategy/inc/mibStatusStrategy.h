#ifndef STATUSSTRATEGY_H
#define STATUSSTRATEGY_H

#include <QObject>
#include <mibJSONProtocol.h>
#include <mibAbstractSocketStrategy.h>
#include <mibMCP3008.h>

#include "mibStatusStrategyGlobal.h"
#include "mibStatusConfigRes.h"
#include "mibStatusReader.h"

namespace mibot
{

class MIBSTATUSSTRATEGYSHARED_EXPORT StatusStrategy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    explicit StatusStrategy(mibot::Connection *connection);
    ~StatusStrategy();

signals:

private slots:
    void onStrategyUpdate();

protected:
    void processNewData(QByteArray);
    bool init();
private:
    JSONProtocol    _json_protocol;

    void fixIfJsonIsCorrupted();
    QJsonObject createRequest(QJsonObject & obj);
    void readValuesToJsonObjec(QJsonObject & obj);
    StatusReader * _status_reader;
    QTimer * _update_timer;
    bool    _auto_send;
    QList<QString> _read_values_filter;
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);

#endif // STATUSSTRATEGY_H
