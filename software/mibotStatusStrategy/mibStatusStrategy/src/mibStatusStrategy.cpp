#include <mibLogger.h>
#include <mibSettingsClient.h>
#include "inc/mibStatusStrategy.h"

using namespace mibot;

StatusStrategy::StatusStrategy(Connection *connection) :
    AbstractSocketStrategy(connection),
    _statusSettigns(nullptr),
    _systemSensorsReader(nullptr),
    _mcp3008Reader (nullptr),
    _gpsSensorsReader(nullptr),
    _auto_send(false)
{
    _update_timer = new QTimer(this);
    connect( _update_timer, SIGNAL(timeout()), this, SLOT(onStrategyUpdate()));
}

StatusStrategy::~StatusStrategy()
{
    if(_systemSensorsReader != nullptr)
    {
        delete _systemSensorsReader;
    }

    if(_mcp3008Reader != nullptr)
    {
        delete _mcp3008Reader;
    }

    if(_gpsSensorsReader != nullptr)
    {
        delete _gpsSensorsReader;
    }

    if(_statusSettigns != nullptr)
        _statusSettigns->Release();
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
    _statusSettigns = SettingsClient::CreateReource<StatusSettings>( _connection->SocketObj->strategyConfig->value );
    if(!_statusSettigns->Sync())
    {
        LOG_ERROR(QString("Can't get status settings '%1'").arg( _connection->SocketObj->strategyConfig->value ));
        _statusSettigns->Release();
        return false;
    }

    if(!SystemSensors::get()->Initialize())
    {
        LOG_ERROR("Can't initialize SystemSensors!");
        return false;
    }

    if(!MCP3008Sensor::get()->Initialize())
    {
        LOG_ERROR("Can't initialize MCP3008Sensor");
        return false;
    }

    if(!GPSSensor::get()->Initialize())
    {
        LOG_ERROR("Can't initialize GPSSensor");
        return false;
    }

    _systemSensorsReader = SystemSensors::get()->GetReader( _statusSettigns->updateRatio->value );
    _mcp3008Reader = MCP3008Sensor::get()->GetReader( _statusSettigns->updateRatio->value );
    _gpsSensorsReader = GPSSensor::get()->GetReader( _statusSettigns->updateRatio->value );

    _update_timer->setInterval( _statusSettigns->internalDelay->value );
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
    QMap<QString,QVariant> values = _systemSensorsReader->ReadAll();
    float accValue  = _mcp3008Reader->Read(
                _statusSettigns->enginesAccuMcp3008Channel->value
                ).toFloat() * _statusSettigns->enginesAccuVScale->value;

    QPointF gpsPos = _gpsSensorsReader->Read("pos").toPointF();

    if(_read_values_filter.empty())
    {
        for(QString k : values.keys())
        {
            QJsonObject obj;
            obj.insert(k,QJsonValue(values[k].toDouble()));
            arr.append( QJsonValue(obj) );
        }

        QJsonObject obj;
        obj.insert("accu_volt", QJsonValue( accValue ));
        arr.append( QJsonValue(obj) );

        QJsonObject obj2;
        obj2.insert("gps_pos_x", QJsonValue(gpsPos.x()));
        obj2.insert("gps_pos_y", QJsonValue(gpsPos.y()));
        arr.append( QJsonValue(obj2) );
    }
    else
    {
        for(QString k : _read_values_filter)
        {
            if(k == "accu_volt")
            {
                QJsonObject obj;
                obj.insert(k,accValue);
                arr.append( QJsonValue(obj) );
                continue;
            }

            if(k == "gps_pos")
            {
                QJsonObject obj2;
                obj2.insert("gps_pos_x", QJsonValue(gpsPos.x()));
                obj2.insert("gps_pos_y", QJsonValue(gpsPos.y()));
                arr.append( QJsonValue(obj2) );
                continue;
            }

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
