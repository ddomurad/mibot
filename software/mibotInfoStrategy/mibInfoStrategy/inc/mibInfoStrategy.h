#ifndef INFOSTRATEGY_H
#define INFOSTRATEGY_H

#include <QObject>
#include <mibJSONProtocol.h>
#include <mibAbstractSocketStrategy.h>

#include "mibInfoStrategyGlobal.h"

namespace mibot
{


class MIBINFOSTRATEGYSHARED_EXPORT InfoStrategy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    explicit InfoStrategy(Connection *connection = 0);
    ~InfoStrategy();

signals:

private slots:
    void onStrategyUpdate();

protected:
    void processNewData(QByteArray data);
    bool init();

private:
    JSONProtocol    _json_protocol;
    QTimer          *_timer;
    QJsonObject createRequest(QJsonObject & obj);
    void getInfoObject(QJsonObject &obj, QString request);


    QJsonArray getSocketInfo();

    void fixIfJsonIsCorrupted();
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);

#endif // INFOSTRATEGY_H
