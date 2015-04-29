#ifndef VIDEOSTRATEGY_H
#define VIDEOSTRATEGY_H

#include <QObject>
#include <mibAbstractSocketStrategy.h>
#include "mibVideoStrategyGlobal.h"

namespace mibot
{

class VideoStrategy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    explicit VideoStrategy(mibot::Connection *connection);
    ~VideoStrategy();

signals:

public slots:
private slots:
    void onFrame(VideoStreamFrameData *frame);

protected:
    void processNewData(QByteArray);
    bool init();
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);

#endif // VIDEOSTRATEGY_H
