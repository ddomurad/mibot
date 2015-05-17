#ifndef PROTOTYPEVIDEOSTRATEGY_H
#define PROTOTYPEVIDEOSTRATEGY_H

#include <QObject>
#include <QThread>

#include "mibPrototypeVideoStrategyGlobal.h"
#include <mibAbstractSocketStrategy.h>
#include <mibJSONProtocol.h>

#include "mibStreamer.h"

namespace mibot
{
#define PVS_WARN_PROC_NORUNNING  0x01

class MIBPROTOTYPEVIDEOSTRATEGYSHARED_EXPORT PrototypeVideoStrategy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    explicit PrototypeVideoStrategy(mibot::Connection *connection);
    ~PrototypeVideoStrategy();

signals:

public slots:

private slots:
    void onStartStreaming();
    void onStopStreaming();
    void onOpenDeviceError();
    void onReadDeviceError();
    void onData(StreamData data);

protected:
    void processNewData(QByteArray);
    bool init();

private:
    QString _last_error;
    QThread * _thread;
    Streamer * _streamer;


    bool startStream(int width, int height);
    bool stopSteam();
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);
#endif // PROTOTYPEVIDEOSTRATEGY_H
