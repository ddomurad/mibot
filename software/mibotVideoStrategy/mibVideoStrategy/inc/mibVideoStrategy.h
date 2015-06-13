#ifndef VIDEOSTRATEGY_H
#define VIDEOSTRATEGY_H

#include <QObject>
#include <mibAbstractSocketStrategy.h>
#include <mibJSONProtocol.h>

#include "mibVideoStrategyGlobal.h"
#include "mibVideoStreamer.h"

namespace mibot
{

class VideoStrategy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    explicit VideoStrategy(Connection *connection);
    ~VideoStrategy();

signals:

public slots:

private slots:
    void onStrategyUpdate();

protected:
    void processNewData(QByteArray);
    bool init();

private:
    JSONProtocol protocol;
    void tryRemoveCoruptedJSONProtocolData();
    void processCommand(QJsonObject &obj);

    VideoStreamer streamer;
    QTimer * timer;

    bool _shuld_stream;
    QList<QJsonObject> _objsToSend;

    void pushStateResponse(QString state, QList<QString> errorList = QList<QString>());
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);

#endif // VIDEOSTRATEGY_H
