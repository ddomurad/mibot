#include <mibLogger.h>
#include <mibSettingsClient.h>
#include "inc/mibStatusStrategy.h"

using namespace mibot;

StatusStrategy::StatusStrategy(Connection *connection) :
    AbstractSocketStrategy(connection),
    _statusSettigns(nullptr),
    _systemSensors(nullptr),
    _arduinoSensorNode (nullptr),
    _auto_send(false)
{
    _update_timer = new QTimer(this);
    connect( _update_timer, SIGNAL(timeout()), this, SLOT(onStrategyUpdate()));
}

StatusStrategy::~StatusStrategy()
{
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

    if(!ArduinoSensorNode::get()->Initialize())
    {
        LOG_ERROR("Can't initialize ArduinoSensorNode");
        return false;
    }

    _systemSensors = SystemSensors::get();
    _arduinoSensorNode = ArduinoSensorNode::get();

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
}

{
  "send_trigger":"manual",
}

{
 "action":"send"
}

{
 "action":"send",
  "send_trigger":"manual",
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
    SystemSensorsReading ssReading = _systemSensors->Readings();
    ArduinoReadings arReadings = _arduinoSensorNode->Readings();

    QJsonObject systemReading;
    systemReading.insert("cpu_temp",QJsonValue(ssReading.cpu_temp));
    systemReading.insert("cpu_usage_server",QJsonValue(ssReading.cpu_usage_server));
    systemReading.insert("cpu_usage_total",QJsonValue(ssReading.cpu_usage_total));
    systemReading.insert("mem_available",QJsonValue(ssReading.mem_available));
    systemReading.insert("mem_usage_server",QJsonValue(ssReading.mem_usage_server));
    systemReading.insert("mem_usage_total",QJsonValue(ssReading.mem_usage_total));

    QJsonObject arduinoReading;
    QJsonArray arduinoReadingAcc;
    QJsonArray arduinoReadingMag;
    QJsonArray arduinoReadingAnalog;

    arduinoReadingAcc.append(QJsonValue(arReadings.acc[0]));
    arduinoReadingAcc.append(QJsonValue(arReadings.acc[1]));
    arduinoReadingAcc.append(QJsonValue(arReadings.acc[2]));

    arduinoReadingMag.append(QJsonValue(arReadings.mag[0]));
    arduinoReadingMag.append(QJsonValue(arReadings.mag[1]));
    arduinoReadingMag.append(QJsonValue(arReadings.mag[2]));

    for(int i=0;i<10;i++)
    {
       if(!arReadings.isAnalogValue[i])
           continue;

       QJsonObject analogObj;
       analogObj.insert("c", i);
       analogObj.insert("v", arReadings.analogValue[i]);
       arduinoReadingAnalog.append(analogObj);
    }

    arduinoReading.insert("acc",QJsonValue(arduinoReadingAcc));
    arduinoReading.insert("mag",QJsonValue(arduinoReadingMag));
    arduinoReading.insert("analog",QJsonValue(arduinoReadingAnalog));
    arduinoReading.insert("us",QJsonValue(arReadings.us));


    obj.insert("system",QJsonValue(systemReading));
    obj.insert("sensors",QJsonValue(arduinoReading));
}

mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new StatusStrategy(connection);
}
