#include <mibLogger.h>
#include <mibGlobalAccess.h>
#include "inc/mibStatusStrategy.h"

using namespace mibot;

StatusStrategy::StatusStrategy(Connection *connection) :
    AbstractSocketStrategy(connection),
    _auto_send(false)
{
    _update_timer = new QTimer(this);
    connect( _update_timer, SIGNAL(timeout()), this, SLOT(onStrategyUpdate()));
}

StatusStrategy::~StatusStrategy()
{
    StatusReader::ReleaseStatusReader( _connection );
}

void StatusStrategy::onStrategyUpdate()
{
    fixIfJsonIsCorrupted();

    if(_json_protocol.ContainsValidJSONObject())
    {
        QJsonObject object = _json_protocol.GetPendingObject();
        fixIfJsonIsCorrupted();

        QJsonObject ret_obj = createRequest(object);
        if(!ret_obj.isEmpty())
        {
            QJsonDocument doc( ret_obj);
            _connection->TcpSocket->write( doc.toJson( QJsonDocument::Compact ) );
        }
    }

    if(_auto_send)
    {
        QJsonObject obj;
        readValuesToJsonObjec(obj);
        QJsonDocument doc(obj);
        _connection->TcpSocket->write( doc.toJson( QJsonDocument::Compact ) );
    }
}

void StatusStrategy::processNewData(QByteArray arr)
{
    _json_protocol.PushData(arr);
    //QThread::msleep( _status_reader->Config()->InternalDelay() );
}

bool StatusStrategy::init()
{
    _status_reader = StatusReader::GetStatusReader(_connection);
    if(_status_reader == nullptr)
    {
        LOG_ERROR("Can't initialize StatusStrategy. (_status_reader == nullptr)");
        return false;
    }

    _update_timer->setInterval( _status_reader->Config()->InternalDelay() );
    _update_timer->start();

    return true;
}

void StatusStrategy::fixIfJsonIsCorrupted()
{
    if(_json_protocol.IsDataCorupted())
    {
        LOG_WARNING("StatusStrategy JSONProtocol handler contains corrupted json data. The handler will try to remove corrupted data.");
        _json_protocol.RemoveCoruptedData();
    }
}

/*
{
  "send_trigger":"auto",
  "interested":["cpu_temp","accu_volt"]
}

{
  "send_trigger":"manual",
  "interested":[]
}

{
 "action":"send"
}

{
 "action":"send",
  "send_trigger":"manual",
  "interested":[]
}

*/

QJsonObject StatusStrategy::createRequest(QJsonObject &obj)
{
    QJsonObject out;

    if(obj["send_trigger"].isString())
    {
        if(obj["send_trigger"].toString() == "manual")
        {
            _auto_send = false;
        }
        else if(obj["send_trigger"].toString() == "auto")
        {
            _auto_send = true;
        }
    }

    if(obj["interested"].isArray())
    {
        _read_values_filter.clear();
        QJsonArray arr = obj["interested"].toArray();
        for(int i=-0; i<arr.count(); i++)
        {
            if(arr.at(i).isString())
            {
                _read_values_filter.append( arr.at(i).toString() );
            }
        }
    }

    if(obj["action"].isString())
    {
        if(obj["action"].toString() == "send")
        {
            if(!_auto_send)
                readValuesToJsonObjec( out );
        }
    }


    return out;
}

void StatusStrategy::readValuesToJsonObjec(QJsonObject &obj)
{
    QJsonArray arr;
    QMap<QString,QVariant> values = _status_reader->Readings();
    if(_read_values_filter.empty())
    {
        for(QString k : values.keys())
        {
            QJsonObject obj;
            obj.insert(k,QJsonValue( values[k].toString() ));
            arr.append( QJsonValue(obj) );
        }
    }
    else
    {
        for(QString k : _read_values_filter)
        {
            if(!values.contains(k))
            {
                QJsonObject obj;
                obj.insert(k,QJsonValue( "" ));
                arr.append( QJsonValue(obj) );
            }
            else
            {
                QJsonObject obj;
                obj.insert(k,QJsonValue(values[k].toString() ));
                arr.append( QJsonValue(obj) );
            }
        }
    }

    obj.insert("reading",QJsonValue(arr));
}

mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new StatusStrategy(connection);
}
