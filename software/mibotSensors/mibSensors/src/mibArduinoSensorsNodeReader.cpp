#include <mibLogger.h>

#include <QMutexLocker>
#include <QMutex>

#include "inc/mibArduinoSensorsNodeReader.h"

using namespace mibot;

ArduinoSensorsNodeReader::ArduinoSensorsNodeReader(QObject *parent) : QObject(parent)
{
    _settings = nullptr;
    _isReading = false;
    _settings_update_timer = new QTimer(this);

    for(int i=0;i<5;i++)
        _last_analog_states[i] = 0xFF;

    _last_gps_state = 0;
    _last_acc_state = 0;
    _last_mag_state = 0;

    connect(this, SIGNAL(StartReaderSignal()), this, SLOT(startReader()));
    connect(this, SIGNAL(StopReaderSignal()), this, SLOT(stopReader()));
    connect(_settings_update_timer, SIGNAL(timeout()), this, SLOT(onSettingsUpdateTimer()));

    _settings_update_timer->start(1000);
}

ArduinoSensorsNodeReader::~ArduinoSensorsNodeReader()
{
    if(_settings != nullptr)
    {
        _settings->Release();
    }
}

bool ArduinoSensorsNodeReader::IsReading()
{
    return _isReading;
}

bool ArduinoSensorsNodeReader::StartReader()
{
    emit StartReaderSignal();
    _startEvent.Wait(2500);
    return IsReading();
}

void ArduinoSensorsNodeReader::StopReader()
{
    emit StopReaderSignal();
}

QMap<QString, float> ArduinoSensorsNodeReader::Readings()
{
    QMutexLocker locker(&_readingMutex);
    QMap<QString, float> tmpReads = _readings;
    return tmpReads;
}

void ArduinoSensorsNodeReader::startReader()
{
    if(_isReading)
        return;

    _settings = SensorsSettings::getGlobal();
    if(!_settings->Sync())
    {
        LOG_ERROR( QString("Can't downlaod SensorSettings: %1").arg(_settings->Resource()));
        _isReading = false;
        _settings->Release();
        _settings = nullptr;
        _startEvent.Open();
        return;
    }

    _serialPort = new QSerialPort(
                _settings->arduinoNodeSerialPortName->value,
                this);

    connect(_serialPort, SIGNAL(readyRead()), this, SLOT(onSerialPortRead()));

    _serialPort->setBaudRate(_settings->arduinoNodeSerialBaudRate->value);
    if(!_serialPort->open(QSerialPort::ReadWrite))
    {
        LOG_ERROR( QString("Can't open serial port (name: %1, baud: %2)")
                   .arg(_settings->arduinoNodeSerialPortName->value)
                   .arg(_settings->arduinoNodeSerialBaudRate->value));

        delete _serialPort;
        _settings->Release();
        _settings = nullptr;
        _isReading = false;
        _startEvent.Open();
        return;
    }

    _isReading = true;
    _startEvent.Open();
}

void ArduinoSensorsNodeReader::stopReader()
{

}

void ArduinoSensorsNodeReader::onSerialPortRead()
{
    QMutexLocker locker(&_readingMutex);

    QByteArray readed = _serialPort->readAll();

    _serialData += readed;
    int index = _serialData.indexOf('\n');
    if(index != -1)
    {
        QString line = _serialData.left(index - 1);
        _serialData = _serialData.remove(0,index+1);
        processData( line );
    }
}

void ArduinoSensorsNodeReader::onSettingsUpdateTimer()
{
    QMutexLocker locker(&_readingMutex);

    if(!_isReading) return;

    if(_settings->enable_acc->value != _last_acc_state)
    {
        _last_acc_state = _settings->enable_acc->value;
        sendFlag(ACC_FLAG_ADDR, _last_acc_state);
    }

    if(_last_acc_state != 49)
    {
        _readings.remove(ACC_X_SENSOR_TAG);
        _readings.remove(ACC_Y_SENSOR_TAG);
        _readings.remove(ACC_Z_SENSOR_TAG);
    }

    if(_settings->enable_mag->value != _last_mag_state)
    {
        _last_mag_state = _settings->enable_mag->value;
        sendFlag(MAG_FLAG_ADDR, _last_mag_state);
    }

    if(_last_mag_state != 49)
    {
        _readings.remove(MAG_X_SENSOR_TAG);
        _readings.remove(MAG_Y_SENSOR_TAG);
        _readings.remove(MAG_Z_SENSOR_TAG);
    }

    if(_settings->enable_gps->value != _last_gps_state)
    {
        _last_gps_state = _settings->enable_gps->value;
        sendFlag(GPS_FLAG_ADDR, _last_gps_state);

    }

    if(_last_gps_state != 49)
    {
        _readings.remove(GPS_LAT_1_SENSOR_TAG);
        _readings.remove(GPS_LAT_2_SENSOR_TAG);
        _readings.remove(GPS_LON_1_SENSOR_TAG);
        _readings.remove(GPS_LON_2_SENSOR_TAG);
        _readings.remove(GPS_DIR_SENSOR_TAG);
        _readings.remove(GPS_SPEED_SENSOR_TAG);
    }

    for(int i=0;i<5;i++)
    {
        if(_settings->analog[i]->value != _last_analog_states[i])
        {
            _last_analog_states[i] = _settings->analog[i]->value;
            sendFlag(ANALOG_BASE_FLAG_ADDR + i, _last_analog_states[i]);
        }

        if(_last_analog_states[i] == 0xFF)
        {
            _readings.remove(QString("a%1").arg(i + 1));
        }
    }
}

void ArduinoSensorsNodeReader::processData(QString str)
{
    if(str.length() == 0) return;
    QCharRef tmpLast = str[str.count() - 1];
    if( tmpLast == '\n' || tmpLast == '\r')
        str = str.trimmed();

    QStringList splited = str.split(' ');
    if(splited.count() != 2)
    {
        LOG_WARNING(QString("Recived line is invalid '%1'").arg(str));
        return;
    }

    if(splited[0] == "GPS")
        processGPS(splited[1]);
    else if(splited[0] == "ACC")
        processAcc(splited[1]);
    else if(splited[0] == "MAG")
        processMag(splited[1]);
    else if(splited[0] == "A1"
            || splited[0] == "A2"
            || splited[0] == "A3"
            || splited[0] == "A4"
            || splited[0] == "A5")
        processAnalog(splited[0], splited[1]);
    else
    {
        LOG_WARNING("Recived unknown value from arduino.");
    }
}

void ArduinoSensorsNodeReader::processAcc(QString value)
{
    updateKey(ACC_X_SENSOR_TAG);
    updateKey(ACC_Y_SENSOR_TAG);
    updateKey(ACC_Z_SENSOR_TAG);

    QStringList values = value.split(":");

    if(values.count() != 3)
    {
        LOG_WARNING( QString("Can't split acc valued into 3 element vector. (data: '%1')").arg(value) );
        return;
    }

    _readings[ACC_X_SENSOR_TAG] = values[0].toFloat();
    _readings[ACC_Y_SENSOR_TAG] = values[1].toFloat();
    _readings[ACC_Z_SENSOR_TAG] = values[2].toFloat();

    //    LOG_DEBUG(
    //                QString("ACC: {%1,%2,%3}")
    //                .arg(_readings[ACC_X_SENSOR_TAG])
    //                .arg(_readings[ACC_Y_SENSOR_TAG])
    //                .arg(_readings[ACC_Z_SENSOR_TAG]));
}

void ArduinoSensorsNodeReader::processMag(QString value)
{
    updateKey(MAG_X_SENSOR_TAG);
    updateKey(MAG_Y_SENSOR_TAG);
    updateKey(MAG_Z_SENSOR_TAG);

    QStringList values = value.split(":");

    if(values.count() != 3)
    {
        LOG_WARNING( QString("Can't split mag valued into 3 element vector. (data: '%1')").arg(value) );
        return;
    }

    _readings[MAG_X_SENSOR_TAG] = values[0].toFloat();
    _readings[MAG_Y_SENSOR_TAG] = values[1].toFloat();
    _readings[MAG_Z_SENSOR_TAG] = values[2].toFloat();

    //    LOG_DEBUG(
    //                QString("MAG: {%1,%2,%3}")
    //                .arg(_readings[MAG_X_SENSOR_TAG])
    //                .arg(_readings[MAG_Y_SENSOR_TAG])
    //                .arg(_readings[MAG_Z_SENSOR_TAG]));
}

void ArduinoSensorsNodeReader::processAnalog(QString channel, QString value)
{
    if(channel != "A1"
            && channel != "A2"
            && channel != "A3"
            && channel != "A4"
            && channel != "A5")
    {
        LOG_WARNING(QString("Received channel name is ivalid: '%1'").arg(channel));
        return;
    }

    channel = channel.toLower();

    updateKey(channel);
    _readings[channel] = value.toFloat();

    //    LOG_DEBUG(
    //                QString("%1: {%2}")
    //                .arg(channel)
    //                .arg(_readings[channel]));
}

void ArduinoSensorsNodeReader::processGPS(QString value)
{
    if(value.length() == 0)
    {
        LOG_WARNING("Empty GPS data");
        return;
    }

    QStringList valueList = value.split(',');
    if(valueList.count() == 1)
    {
        //LOG_WARNING(QString("Invalid gps data: '%1'").arg(value));
        return;
    }

    if(valueList[0] == "GPGGA")
    {
        if(valueList.count()!=15)
        {
            return;
        }
        else
        {
            QStringList lat_split = valueList[2].split('.');
            QStringList lon_split = valueList[4].split('.');
            _readings[GPS_LAT_1_SENSOR_TAG] = lat_split[0].toFloat();
            _readings[GPS_LAT_2_SENSOR_TAG] = lat_split[1].toFloat();
            _readings[GPS_LON_1_SENSOR_TAG] = lon_split[0].toFloat();
            _readings[GPS_LON_2_SENSOR_TAG] = lon_split[1].toFloat();

            return;
        }
    }
    else if(valueList[0] == "GPVTG")
    {
        if(valueList.count()!=10)
        {
            //LOG_WARNING(QString("Invalid GPVTG gps data: '%1'").arg(value));
            return;
        }else
        {
            // LOG_DEBUG(QString("Valid GPVTG gps data: '%1'").arg(value));
            _readings[GPS_SPEED_SENSOR_TAG] = valueList[7].toFloat();
            _readings[GPS_DIR_SENSOR_TAG] = valueList[1].toFloat();

            //            LOG_DEBUG(
            //                        QString("GPS_SPEED: S:%1, D:%2")
            //                        .arg(_readings[GPS_SPEED_SENSOR_TAG])
            //                        .arg(_readings[GPS_DIR_SENSOR_TAG]));
            return;
        }
    }
    else
    {
        //LOG_WARNING(QString("Unsuported gps data: '%1'").arg(value));
        return;
    }
}

void ArduinoSensorsNodeReader::updateKey(QString key)
{
    if(!_readings.contains(key))
        _readings.insert(key,0);
}

void ArduinoSensorsNodeReader::sendFlag(int addr, int value)
{
    QByteArray data;
    data.push_back('>');
    data.push_back( (unsigned char)(addr) + '0' );
    data.push_back( (unsigned char)(value) );

    if(_serialPort->write( data ) == -1)
    {
        LOG_ERROR("Can't write flags to arduino node!");
    }
}

