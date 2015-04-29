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

void VideoStrategy::onFrame(QByteArray *arr)
{
    _connection->TcpSocket->write( *arr );
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

    connect(VideoStreamer::get(),SIGNAL(OnFrameData(QByteArray*)),this,SLOT(onFrame(QByteArray*)));
    VideoStreamer::get()->Attach(_connection->TcpSocket);
    return true;
}


mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new VideoStrategy(connection);
}
