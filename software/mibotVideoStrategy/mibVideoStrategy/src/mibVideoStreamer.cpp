#include <unistd.h>
#include <time.h>
#include <mibLogger.h>
#include "inc/mibVideoStreamer.h"

#include <mibGlobalAccess.h>

using namespace mibot;

VideoStreamer *VideoStreamer::get()
{
    static VideoStreamer instance;
    return &instance;
}

VideoStreamer::~VideoStreamer()
{
    if(config != nullptr)
        delete config;

    pthread_mutex_destroy(&_connections_update_mutex);
}

bool VideoStreamer::Init(QString configId)
{
    if(isInitialized == true) return true;


    ResourceWrapper<VideoConfigRes> *repo = new ResourceWrapper<VideoConfigRes>(GlobalAccess::get().Repository());
    VideoConfigRes * cfg = repo->getByID( configId );
    delete repo;

    if(cfg == nullptr)
    {
        LOG_ERROR( QString("Can't read VideoStreamer configuration id: %1").arg( configId ));
        return false;
    }

    config = cfg;
    isInitialized = true;
    return true;
}

void VideoStreamer::Attach(QTcpSocket *connection)
{
    lockConnectionList();

    for(int i=0;i<connections.count();i++)
        if(connections[i].socket == connection)
            return;

    TimedConnection tc;
    tc.socket = connection;
    tc.timer.restart();

    connections.append( tc );

    unlockConnectionList();

    if(shallStreamingThreadRun == false && connections.count() != 0)
        startStreaming();
}

void VideoStreamer::Detach(QTcpSocket *connection)
{
    lockConnectionList();
    for(int i=0;i<connections.count();i++)
        if(connections[i].socket == connection)
        {
            connections.removeAt(i);
            unlockConnectionList();

            if(shallStreamingThreadRun == true && connections.count() == 0)
                stopStreaming();

            return;
        }
    unlockConnectionList();
}

void VideoStreamer::KeepAlive(QTcpSocket *connection)
{
    for(int i=0;i<connections.count();i++)
    {
        if(connections[i].socket == connection)
        {
            connections[i].timer.restart(); return;
        }
    }
}

bool VideoStreamer::startStreaming()
{
    if(_thread != 0)
        return false;

    if(timedLock(5) == false)
    {
        LOG_ERROR("Can't lock mutex.");
        return false;
    }

    LOG_INFO("Starting streamer");

    shallStreamingThreadRun = true;
    pthread_create( &_thread, 0, __streamThread,0);

    unlock();
    return true;
}

void VideoStreamer::stopStreaming()
{
    LOG_INFO("Stoping streamer");
    shallStreamingThreadRun = false;
    void * ret;

    timespec time;
    clock_gettime( CLOCK_REALTIME, &time);
    time.tv_sec += 15;

    pthread_timedjoin_np(_thread,&ret,&time);
    _thread = 0;
}

void VideoStreamer::lockConnectionList()
{
    shallUpdateConnectionList = true;

    if(timedLock(5) == false)
    {
        LOG_WARNING("Can't lock mutex for clients list udpate.");
        return;
    }
}

void VideoStreamer::unlockConnectionList()
{
    shallUpdateConnectionList = false;
    unlock();
    while(isStreaming == false && shallStreamingThreadRun == true) usleep(1000);
}

bool VideoStreamer::timedLock(int sec)
{
    timespec time;
    clock_gettime( CLOCK_REALTIME, &time);
    time.tv_sec += sec;
    return pthread_mutex_timedlock(&_connections_update_mutex, &time) == 0;
}

void VideoStreamer::unlock()
{
    pthread_mutex_unlock(&_connections_update_mutex);
}

void VideoStreamer::streamThread()
{
    if(timedLock(15) == false)
    {
        LOG_ERROR("Can't lock mutex at thread start.");
        shallStreamingThreadRun = false;
        return;
    }

    LOG_INFO("Streaming thread started.");

    if(v4l2_wrapper != nullptr)
        delete v4l2_wrapper;

    v4l2_wrapper = new V4L2Wrapper();

    if(v4l2_wrapper->Open("/dev/video0",1,640,480) == false)
    {
        delete v4l2_wrapper;
        v4l2_wrapper = nullptr;
        shallStreamingThreadRun = false;
    }else
    {
        if(v4l2_wrapper->Start() == false)
        {
            v4l2_wrapper->Close();
            delete v4l2_wrapper;
            v4l2_wrapper = nullptr;
            shallStreamingThreadRun = false;
        }
    }


    isStreaming = true;
    while(shallStreamingThreadRun)
    {
        // running

        if(shallUpdateConnectionList == true)
        {
            isStreaming = false;
            unlock();
            while(shallUpdateConnectionList == true
                  && shallStreamingThreadRun == true)
                usleep(1000);

            if(timedLock(5) == false)
            {
                LOG_ERROR("Can't lock mutex.");
                shallStreamingThreadRun = false;
                break;
            }

            isStreaming = true;
        }else
        {
            unsigned int data_len = 0;
            char * data = v4l2_wrapper->GrabNext(&data_len);
            if(data == 0)
            {
                LOG_WARNING("Can't grab next freme.");
            }else
            {
                QByteArray arr = QByteArray::fromRawData(data, data_len);
                emit OnFrameData(&arr);
                /*for(int i=0;i<connections.count();i++)
                {
                    connections[i].socket->write( data , data_len);
                }*/
            }
        }
    }

    if(v4l2_wrapper != nullptr)
    {
        v4l2_wrapper->Stop();
        v4l2_wrapper->Close();
        delete v4l2_wrapper;
        v4l2_wrapper = nullptr;
    }

    isStreaming = false;

    LOG_INFO("Streaming thread stoped.");
    unlock();
}

void *VideoStreamer::__streamThread(void *)
{
    VideoStreamer::get()->streamThread();
    return 0;
}

VideoStreamer::VideoStreamer() :
    config(nullptr),
    _thread(0),
    shallStreamingThreadRun(false),
    shallUpdateConnectionList(false),
    isStreaming(false),
    isInitialized(false),
    v4l2_wrapper(nullptr)
{
    pthread_mutex_init(&_connections_update_mutex,0);
}

