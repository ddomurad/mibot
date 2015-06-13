#include "inc/mibVideoStrategy.h"
#include <mibLogger.h>

using namespace mibot;


VideoStrategy::VideoStrategy(Connection *connection) :
    AbstractSocketStrategy(connection),
   streamer(VideoStreamer())
{
    _shuld_stream = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onStrategyUpdate()));

    timer->start(1000);
}

VideoStrategy::~VideoStrategy()
{
    streamer.StreamStop();
}

void VideoStrategy::onStrategyUpdate()
{
    bool _shall_return = false;
    if(protocol.ContainsValidJSONObject())
    {
        QJsonObject obj = protocol.GetPendingObject();
        processCommand(obj);
        tryRemoveCoruptedJSONProtocolData();

        _shall_return = true;
    }
    if(_objsToSend.count() != 0)
    {
        QJsonObject obj = _objsToSend.first();
        _objsToSend.removeAt(0);

        QJsonDocument doc(obj);
        _connection->TcpSocket->write(doc.toJson(QJsonDocument::Compact));

    }

    if(_shall_return) return;
    if(_shuld_stream == false) return;
    if(_shuld_stream && streamer.IsStreaming()) return;

    _shuld_stream = false;

    streamer.StreamStop();
    QList<QString> errors = streamer.Errors();

    LOG_ERROR("Streamer does not stream!");
    for(int i=0; i<errors.count(); i++)
        LOG_ERROR(errors.at(i));

    pushStateResponse("Stoped",errors);

    streamer.ClearErrors();
}

void VideoStrategy::processNewData(QByteArray data)
{
    protocol.PushData(data);
    tryRemoveCoruptedJSONProtocolData();
}

bool VideoStrategy::init()
{
    return true;
}

void VideoStrategy::tryRemoveCoruptedJSONProtocolData()
{
    if(protocol.IsDataCorupted())
    {
        protocol.RemoveCoruptedData();
        if(protocol.IsDataCorupted())
        {
            LOG_WARNING("Can't remove json procol corupted data. All data is removed.");
            protocol.RemoveAllData();
        }
        else
        {
            LOG_WARNING("Corupted protocol data removed.");
        }
    }
}

void VideoStrategy::processCommand(QJsonObject &obj)
{
    if(obj["stream"].isString())
    {
        if(obj["stream"].toString() == "enable")
        {
            QString addr = obj["addr"].toString();
            int port = obj["port"].toInt();
            QString device = obj["device"].toString();
            int w = obj["width"].toInt();
            int h = obj["height"].toInt();
            int bc = obj["buffers"].toInt();
            QString fmt = obj["format"].toString();

            LOG_INFO(QString("Starting stream (Addr: %1, Port: %2, Device: %3, Width: %4, Height: %5, Buffers: %6, Format: %7)")
                     .arg(addr).arg(port).arg(device).arg(w).arg(h).arg(bc).arg(fmt));

            if(!streamer.StreamStart(addr,port,device,w,h,bc, fmt))
            {
                LOG_ERROR(QString("Can't start stream(Addr: %1, Port: %2, Device: %3, Width: %4, Height: %5, Buffers: %6, Format: %7)")
                         .arg(addr).arg(port).arg(device).arg(w).arg(h).arg(bc).arg(fmt));

                QList<QString> errors = streamer.Errors();
                for(int i=0;i<errors.count(); i++)
                    LOG_ERROR(errors.at(i));

                pushStateResponse("Stoped", errors);
                streamer.ClearErrors();
                _shuld_stream = false;
            }
            else
            {
                _shuld_stream = true;
                pushStateResponse("Running");
            }

            return;
        }

        if(obj["stream"].toString() == "disable")
        {
            LOG_INFO("Stoping VideoSteam.");
            streamer.StreamStop();

            QList<QString> errors = streamer.Errors();
            for(int i=0;i<errors.count(); i++)
                LOG_ERROR(errors.at(i));

            pushStateResponse("Stoped", errors);
            streamer.ClearErrors();

            _shuld_stream = false;
        }
    }
}

void VideoStrategy::pushStateResponse(QString state, QList<QString> errorList)
{
    QJsonObject resp;
    resp.insert("SteamState",QJsonValue(state));
    QJsonArray error_array;

    if(errorList.count() != 0)
    {
        for(int i=0; i<errorList.count(); i++)
            error_array.append(QJsonValue(errorList.at(i)));

        resp.insert("Errors", QJsonValue(error_array));
    }

    _objsToSend.append( resp );
}


mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new VideoStrategy(connection);
}


/*

{
"stream":"enable",
"device":"/dev/video0",
"addr":"localhost",
"port":10500,
"width":800,
"height":600,
"buffers":3,
"format":"yuv420m"
}

{
    "stream":"disable"
}
*/
