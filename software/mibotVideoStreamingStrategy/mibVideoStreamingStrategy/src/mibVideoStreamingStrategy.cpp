#include <mibLogger.h>
#include "../inc/mibVideoStreamingStrategy.h"

using namespace mibot;

VideoStreamingStrategy::VideoStreamingStrategy(Connection *connection)
    :AbstractSocketStrategy(connection)
{
    _streamer = new VideoProcessRunner(this);

     _width = 800;
     _height = 600;
     _fps = 20;
     _port = 20100;

    connect(_streamer, SIGNAL(OnRead(QString)), this, SLOT(OnData(QString)));
    connect(_streamer, SIGNAL(OnStop(int)), this, SLOT(OnStoped(int)));
    connect(_streamer, SIGNAL(OnStart()), this, SLOT(OnStarted()));
}

VideoStreamingStrategy::~VideoStreamingStrategy()
{

}

void VideoStreamingStrategy::OnData(QString data)
{
    QJsonObject obj;
    obj.insert("process-data",QJsonValue(data));

    QJsonDocument doc( obj );
    _connection->TcpSocket->write( doc.toJson(QJsonDocument::Compact) );
}

void VideoStreamingStrategy::OnStarted()
{
    QJsonObject obj;
    obj.insert("process-state",QJsonValue("started"));

    QJsonDocument doc( obj );
    _connection->TcpSocket->write( doc.toJson(QJsonDocument::Compact) );
}

void VideoStreamingStrategy::OnStoped(int c)
{
    QJsonObject obj;
    obj.insert("process-state",QJsonValue("stoped"));
    obj.insert("exit-code",QJsonValue(c));

    QJsonDocument doc( obj );
    _connection->TcpSocket->write( doc.toJson(QJsonDocument::Compact) );
}

void VideoStreamingStrategy::processNewData(QByteArray data)
{
    fixIfJsonIsCorrupted();
    _json_protocol.PushData( data );
    fixIfJsonIsCorrupted();

    while(_json_protocol.ContainsValidJSONObject())
        procedureCommand(_json_protocol.GetPendingObject());
}

bool VideoStreamingStrategy::init()
{
    return true;
}

void VideoStreamingStrategy::fixIfJsonIsCorrupted()
{
    if(_json_protocol.IsDataCorupted())
    {
        LOG_WARNING("StatusStrategy JSONProtocol handler contains corrupted json data. The handler will try to remove corrupted data.");
        _json_protocol.RemoveCoruptedData();
    }
}

void VideoStreamingStrategy::procedureCommand(QJsonObject obj)
{
    if(obj["width"].isDouble())
        _width = obj["width"].toInt();

    if(obj["height"].isDouble())
        _height = obj["height"].toInt();

    if(obj["fps"].isDouble())
        _fps = obj["fps"].toInt();

    if(obj["port"].isDouble())
        _port = obj["port"].toInt();

    if(obj.contains("start"))
    {
        if(!_streamer->Start(_width, _height, _fps, _port))
            LOG_ERROR("Can't start streamer !");
        else
            LOG_INFO("Streamer started !");
    }

    if(obj.contains("stop"))
    {
        _streamer->Stop();
        LOG_INFO("Streamer stoped !");
    }
}

mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new VideoStreamingStrategy(connection);
}
