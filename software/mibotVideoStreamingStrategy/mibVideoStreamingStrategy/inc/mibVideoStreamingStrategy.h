#ifndef MIBVIDEOSTREAMINGSTRATEGY_H
#define MIBVIDEOSTREAMINGSTRATEGY_H

#include <QObject>
#include <mibAbstractSocketStrategy.h>
#include <mibJSONProtocol.h>

#include "mibVideoStreamingStrategyGlobal.h"
#include "mibVideoProcessRunner.h"

namespace mibot
{
class VideoStreamingStrategy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    VideoStreamingStrategy(Connection *connection);
    ~VideoStreamingStrategy();

    // AbstractSocketStrategy interface
private slots:
    void OnData(QString data);
    void OnStarted();
    void OnStoped(int c);

protected:
    void processNewData(QByteArray);
    bool init();

private:
    void fixIfJsonIsCorrupted();
    void procedureCommand(QJsonObject obj);

    JSONProtocol _json_protocol;

    VideoProcessRunner * _streamer;

    int _width;
    int _height;
    int _fps;
    int _port;
};
}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);

#endif // MIBVIDEOSTREAMINGSTRATEGY_H
