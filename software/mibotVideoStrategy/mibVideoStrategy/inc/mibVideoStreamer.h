#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include "mibVideoStrategyGlobal.h"
#include "mibVideoConfigRes.h"
#include <QTcpSocket>
#include <QElapsedTimer>

#include <thread>
#include <mutex>
#include "mibV4L2Wrapper.h"

namespace mibot
{

class VideoStreamer : public QObject
{
    Q_OBJECT
public:
    static VideoStreamer * get();
    ~VideoStreamer();

    bool Init(QString configId);
    void Attach(QTcpSocket*);
    void Detach(QTcpSocket*);
    void KeepAlive(QTcpSocket*);

signals:
    void OnFrameData(VideoStreamFrameData *arr);

private:

    class TimedConnection
    {
    public:
        QTcpSocket * socket;
        QElapsedTimer timer;
    };

    bool startStreaming();
    void stopStreaming();

    void lockConnectionList();
    void unlockConnectionList();

    bool timedLock(int sec);
    void unlock();

    void  streamThread();
    static void * __streamThread(void *);

    VideoStreamer();

    VideoConfigRes *config;
    QList<TimedConnection> connections;

    pthread_mutex_t _connections_update_mutex;
    pthread_t _thread;

    bool shallStreamingThreadRun;
    bool shallUpdateConnectionList;
    bool isStreaming;
    bool isInitialized;

    V4L2Wrapper * v4l2_wrapper;
};

}

#endif // VIDEOSTREAMER_H
