#include "inc/mibVideoStreamer.h"
#include "inc/mibVideoStrategy.h"

#include <mibLogger.h>

using namespace mibot;


VideoStrategy::VideoStrategy(Connection *connection) :
    AbstractSocketStrategy(connection)
{}

VideoStrategy::~VideoStrategy()
{
    if(_connection != nullptr)
        VideoStreamer::get()->Detach(_connection->TcpSocket);
}

void VideoStrategy::onFrame(VideoStreamFrameData *frame)
{
    qDebug() << frame->size;
    _connection->TcpSocket->write( frame->ptr, frame->size );
}

void VideoStrategy::processNewData(QByteArray)
{
    VideoStreamer::get()->KeepAlive(_connection->TcpSocket);
}

bool VideoStrategy::init()
{
    if(VideoStreamer::get()->Init(_connection->SocketObj()->StrategyConfig()) == false)
    {
        LOG_ERROR("Can'initialize video streamer.");
        return false;
    }

    connect(VideoStreamer::get(),SIGNAL(OnFrameData(VideoStreamFrameData*)),this,SLOT(onFrame(VideoStreamFrameData*)));
    VideoStreamer::get()->Attach(_connection->TcpSocket);
    return true;
}


mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new VideoStrategy(connection);
}
