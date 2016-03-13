#include <QStringBuilder>
#include <mibLogger.h>
#include <mibSettingsClient.h>
#include "inc/mibStatusStrategy.h"

using namespace mibot;

StatusStrategy::StatusStrategy(Connection *connection) :
    AbstractSocketStrategy(connection),
    _statusSettigns(nullptr),
    _systemSensors(nullptr),
    _arduinoSensorNode (nullptr),
    _gpsSensor(nullptr),
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

        QString dataToSend  = createResponse(object);
        if(!dataToSend.isEmpty())
            _connection->TcpSocket->write( dataToSend.toUtf8() );
    }

    if(_auto_send)
        _connection->TcpSocket->write( getStringToSend().toUtf8() );
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

    if(!GPSSensor::get()->Start())
    {
        LOG_ERROR("Can't initialzie GPSSensor");
        return false;
    }

    _systemSensors = SystemSensors::get();
    _arduinoSensorNode = ArduinoSensorNode::get();
    _gpsSensor = GPSSensor::get();

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

QString StatusStrategy::createResponse(QJsonObject &obj)
{
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
                getStringToSend();
        }
    }

    if(obj["piezo"].isBool())
    {
        bool piezoState = obj["piezo"].toBool();
        _arduinoSensorNode->SetPiezo(piezoState);
    }

    return "";
}

QString StatusStrategy::getStringToSend()
{
    SystemSensorsReading ssReading = _systemSensors->Readings();
    ArduinoReadings arReadings = _arduinoSensorNode->Readings();
    GPSData gpsData = _gpsSensor->Readings();

    QString outStr = "{\"sensors\":{\"acc\":[";
    outStr = outStr % QString::number(arReadings.acc[0]) % "," % QString::number(arReadings.acc[1]) % "," % QString::number(arReadings.acc[2]) % "]";
    outStr = outStr % ",\"analog\":[";

    bool addComa = false;
    for(int i=0;i<10;i++)
    {
       if(!arReadings.isAnalogValue[i])
           continue;

       if(addComa)
           outStr = outStr % ",";
       addComa = true;
       outStr = outStr % "{\"c\":" % QString::number(i) % ",\"v\":" + QString::number(arReadings.analogValue[i]) % "}";
    }

    outStr = outStr % "],\"mag\":[";
    outStr = outStr % QString::number(arReadings.mag[0]) % "," % QString::number(arReadings.mag[1]) % "," % QString::number(arReadings.mag[2]) % "]";
    outStr = outStr % ",\"us\":" % QString::number(arReadings.us) % "},\"system\":{";
    outStr = outStr % "\"cpu_temp\":" % QString::number(ssReading.cpu_temp);
    outStr = outStr % ",\"cpu_usage_server\":" % QString::number(ssReading.cpu_usage_server);
    outStr = outStr % ",\"cpu_usage_total\":" % QString::number(ssReading.cpu_usage_total);
    outStr = outStr % ",\"mem_available\":" % QString::number(ssReading.mem_available);
    outStr = outStr % ",\"mem_usage_server\":" % QString::number(ssReading.mem_usage_server);
    outStr = outStr % ",\"mem_usage_total\":" % QString::number(ssReading.mem_usage_total) % "}";
    outStr = outStr % ",\"gps\":{";
    outStr = outStr % "\"fix\":" % QString::number(gpsData.isValid ? 1 : 0) ;
    outStr = outStr % ",\"latitude\":" % QString::number(gpsData.position.latitude) ;
    outStr = outStr % ",\"lognitude\":" % QString::number(gpsData.position.lognitude) ;
    outStr = outStr % ",\"speed_kmh\":" % QString::number(gpsData.movement.speedKmh) ;
    outStr = outStr % ",\"cource\":" % QString::number(gpsData.movement.course) ;
    outStr = outStr % "}}";

    return outStr;
}

mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new StatusStrategy(connection);
}
