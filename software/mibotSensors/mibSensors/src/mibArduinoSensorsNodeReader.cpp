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
    _motor_lock_timer = new QTimer(this);

    connect(this, SIGNAL(StartReaderSignal()), this, SLOT(startReader()));
    connect(this, SIGNAL(StopReaderSignal()), this, SLOT(stopReader()));
    connect(_settings_update_timer, SIGNAL(timeout()), this, SLOT(onSettingsUpdateTimer()));
    connect(_motor_lock_timer, SIGNAL(timeout()), this, SLOT(onResetMotorLock()));
    connect(this, SIGNAL(SetPiezo(bool)),this,SLOT(onSetPiezo(bool)));

    _last_acc_state = 0;
    _last_mag_state = 0;

     for(int i=0;i<3;i++)
     {
         _readings.acc[i] = 0;
         _readings.mag[i] = 0;
     }

     for(int i=0;i<10;i++)
     {
         _last_analog_states[i] = 0;
         _readings.analogValue[i] = 0.0f;
         _readings.isAnalogValue[i] = false;
     }

     _readings.us = 0;

    _settings_update_timer->start(1000);
    _motor_lock_timer->start(220);

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

ArduinoReadings ArduinoSensorsNodeReader::Readings()
{
    QMutexLocker locker(&_readingMutex);
    return _readings;
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
    if(!_isReading)
        return;
    QMutexLocker locker(&_readingMutex);

    while(_serialPort->canReadLine())
    {
        QByteArray line = _serialPort->readLine(256);
        processData( line );
    }
}

void ArduinoSensorsNodeReader::onSettingsUpdateTimer()
{
    QMutexLocker locker(&_readingMutex);

    if(!_isReading) return;

    for(int i=0;i<10;i++)
    {
        if(_settings->analog[i]->value != _last_analog_states[i])
        {
            _last_analog_states[i] = _settings->analog[i]->value;
            SendCommand(_last_analog_states[i] == 0 ? 'a' : 'A','0' + i);
            _readings.isAnalogValue[i] = _last_analog_states[i] != 0;
        }
    }
}

void ArduinoSensorsNodeReader::onResetMotorLock()
{
    QMutexLocker lockar(&_sendingMutex);
    QByteArray data;
    data.push_back('*');

    if(_serialPort->write( data ) == -1)
        LOG_ERROR("Can't write MotorlLockReset to arduino node!");
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

    else if(splited[0] == "US")
        processUs(splited[1]);
    else if(splited[0] == "ACC")
        processAcc(splited[1]);
    else if(splited[0] == "MAG")
        processMag(splited[1]);
    else if(splited[0] == "A0"
            || splited[0] == "A1"
            || splited[0] == "A2"
            || splited[0] == "A3"
            || splited[0] == "A4"
            || splited[0] == "A5"
            || splited[0] == "A6"
            || splited[0] == "A7"
            || splited[0] == "A8"
            || splited[0] == "A9")
        processAnalog(splited[0][1], splited[1]);
    else
    {
        LOG_WARNING("Recived unknown value from arduino.");
    }
}

void ArduinoSensorsNodeReader::processUs(QString value)
{
    QStringList values = value.split(":");
    if(values.count() != 2)
    {
        LOG_WARNING( QString("Can't split UltraSonar valued into 2 element vector. (data: '%1')").arg(value) );
        return;
    }

    if(values[0] != "0")
    {
        LOG_WARNING( QString("Only 0 channel UltraSonar is supported. (data: '%1')").arg(values[0]) );
        return;
    }

    _readings.us = values[1].toInt();
}

void ArduinoSensorsNodeReader::processAcc(QString value)
{
    QStringList values = value.split(":");

    if(values.count() != 3)
    {
        LOG_WARNING( QString("Can't split acc valued into 3 element vector. (data: '%1')").arg(value) );
        return;
    }

    int x_index = _settings->accXAxisIndex->value;
    int y_index = _settings->accYAxisIndex->value;
    int z_index = _settings->accZAxisIndex->value;
    if(x_index < 0 || y_index < 0 || z_index < 0
            || x_index > 2 || y_index > 2 || z_index > 2)
    {
        LOG_WARNING("Invalid acc axis indices");
        return;
    }

    double scale = _settings->accScale->value;

    _readings.acc[0] = _accFilter[0].Filter(values[x_index].toDouble()) * scale;
    _readings.acc[2] = _accFilter[1].Filter(values[z_index].toDouble()) * scale;
    _readings.acc[1] = _accFilter[2].Filter(values[y_index].toDouble()) * scale;
}

void ArduinoSensorsNodeReader::processMag(QString value)
{
    QStringList values = value.split(":");

    if(values.count() != 3)
    {
        LOG_WARNING( QString("Can't split mag valued into 3 element vector. (data: '%1')").arg(value) );
        return;
    }

    int x_index = _settings->magXAxisIndex->value;
    int y_index = _settings->magYAxisIndex->value;
    int z_index = _settings->magZAxisIndex->value;
    if(x_index < 0 || y_index < 0 || z_index < 0
            || x_index > 2 || y_index > 2 || z_index > 2)
    {
        LOG_WARNING("Invalid mag axis indices");
        return;
    }

    _readings.mag[0] = values[x_index].toFloat();
    _readings.mag[1] = values[z_index].toFloat();
    _readings.mag[2] = values[y_index].toFloat();
}

void ArduinoSensorsNodeReader::processAnalog(QCharRef channel, QString value)
{
    int cn = (int)(channel.toLatin1() - '0');
    if(cn < 0 || cn > 9)
     {
        LOG_ERROR("Channel number invalid");
        return;
    }

    _readings.analogValue[cn] = value.toFloat() * _settings->analogFactor[cn]->value;

    //    LOG_DEBUG(
    //                QString("%1: {%2}")
    //                .arg(channel)
    //                .arg(_readings[channel]));
}

void ArduinoSensorsNodeReader::SendCommand(char cmd, char value)
{
    QMutexLocker lockar(&_sendingMutex);
    QByteArray data;
    data.push_back('!');
    data.push_back('>');
    data.push_back( cmd);
    data.push_back( value);

    if(_serialPort->write( data ) == -1)
    {
        LOG_ERROR("Can't write flags to arduino node!");
    }
}

bool ArduinoSensorsNodeReader::CalibrateAcc(double new_acc_scale)
{
    _settings->accScale->value = new_acc_scale;

    if(!_settings->Upload(SO_DEF_SYNC_TIME))
    {
        LOG_ERROR("Can't upload sensor setting");
        return false;
    }

    return true;
}

void ArduinoSensorsNodeReader::onSetPiezo(bool state)
{
    SendCommand('P', state == true ? '1' : '0');
}

