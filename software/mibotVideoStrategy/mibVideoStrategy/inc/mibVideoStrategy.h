#ifndef VIDEOSTRATEGY_H
#define VIDEOSTRATEGY_H

#include <QObject>
#include <mibAbstractSocketStrategy.h>

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
    void onFrame(QByteArray *);

protected:
    void processNewData(QByteArray);
    bool init();
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);

#endif // VIDEOSTRATEGY_H
