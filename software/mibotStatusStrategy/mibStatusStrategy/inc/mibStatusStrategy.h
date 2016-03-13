#ifndef STATUSSTRATEGY_H
#define STATUSSTRATEGY_H

#include <QObject>
#include <mibJSONProtocol.h>
#include <mibAbstractSocketStrategy.h>

#include <mibSensors.h>
#include "mibStatusStrategyGlobal.h"
#include "mibStatusSettings.h"

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
    QString createResponse(QJsonObject & obj);
    QString getStringToSend();

    StatusSettings * _statusSettigns;
    SystemSensors *_systemSensors;
    ArduinoSensorNode * _arduinoSensorNode;
    GPSSensor * _gpsSensor;

    QTimer * _update_timer;
    bool    _auto_send;
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);

#endif // STATUSSTRATEGY_H
