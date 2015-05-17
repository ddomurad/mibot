#include <mibLogger.h>
#include "inc/mibPrototypeVideoStrategy.h"

using namespace mibot;

PrototypeVideoStrategy::PrototypeVideoStrategy(mibot::Connection *connection)
    : AbstractSocketStrategy(connection)
{
    _thread = new QThread(this);
    _streamer = new Streamer();

    _streamer->moveToThread( _thread );
    connect(_thread,SIGNAL(started()), _streamer, SLOT(StreamStart()));
    connect(_thread,SIGNAL(finished()), _streamer, SLOT(StreamStop()));

    connect(_streamer,SIGNAL(StreamingStarted()), this, SLOT(onStartStreaming()));
    connect(_streamer,SIGNAL(StreamingStoped()), this, SLOT(onStopStreaming()));
    connect(_streamer,SIGNAL(DeviceReadError()), this, SLOT(onReadDeviceError()));
    connect(_streamer,SIGNAL(OpenDeviceError()), this, SLOT(onOpenDeviceError()));
    connect(_streamer,SIGNAL(DataReady(StreamData)), this, SLOT(onData(StreamData)));
}

PrototypeVideoStrategy::~PrototypeVideoStrategy()
{
    stopSteam();
    if(_streamer != 0)
        delete _streamer;
}

void PrototypeVideoStrategy::onStartStreaming()
{
    LOG_INFO("Video streaming started.");
}

void PrototypeVideoStrategy::onStopStreaming()
{
    LOG_INFO("Video streaming stoped.");
}

void PrototypeVideoStrategy::onOpenDeviceError()
{
    LOG_ERROR("Open video device error.");
}

void PrototypeVideoStrategy::onReadDeviceError()
{
    LOG_ERROR("Read data from video device error.");
}

void PrototypeVideoStrategy::onData(StreamData data)
{
    _connection->TcpSocket->write( data.data, data.data_len );
}

void PrototypeVideoStrategy::processNewData(QByteArray)
{}

bool PrototypeVideoStrategy::init()
{
    startStream(800, 600);
    return true;
}


bool PrototypeVideoStrategy::startStream(int width, int height)
{
    if(_thread->isRunning())
    {
        _last_error = "Thread still running.";
        return false;
    }

    _streamer->SetParams("/dev/video0",width,height);

    _thread->start();
    return true;
}

bool PrototypeVideoStrategy::stopSteam()
{
    if(!_thread->isRunning())
    {
        _last_error = "Thrad alrteady stoped.";
        return false;
    }

    _thread->exit();
    return true;
}



mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new PrototypeVideoStrategy(connection);
}
