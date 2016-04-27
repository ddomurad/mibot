#include <mibLogger.h>
#include "inc/mibotGPSSensor.h"

using namespace mibot;

static GPSSensor * _gps_sensor_obj = nullptr;

GPSSensor::GPSSensor(QObject *parent) :
    QObject(parent)
{
    _is_started = false;
    _serialPort = nullptr;
    _settings = nullptr;
}

GPSSensor *GPSSensor::get()
{
    if(_gps_sensor_obj == nullptr)
        _gps_sensor_obj = new GPSSensor(nullptr);

    return _gps_sensor_obj;
}

GPSSensor::~GPSSensor()
{
    if(_settings != nullptr)
    {
        _settings->Release();
    }
}

bool GPSSensor::Start()
{
    if(_is_started)
    {
        LOG_WARNING("GPS sensor already strated");
        return true;
    }

    LOG_INFO("Starting GPS sensor");

    if(_settings == nullptr)
        _settings = SensorsSettings::getGlobal();

    if(!_settings->Sync())
    {
        LOG_ERROR( QString("Can't sync SensorSettings: %1").arg(_settings->Resource()));
        _settings->Release();
        _settings = nullptr;
        return false;
    }

    _serialPort = new QSerialPort(
                _settings->gpsSerialPortName->value,this);
    _serialPort->setBaudRate(_settings->gpsSerialBaudRate->value);
    connect(_serialPort, SIGNAL(readyRead()), this, SLOT(onSerialData()));

    if(!_serialPort->open(QSerialPort::ReadWrite))
    {
        LOG_ERROR( QString("Can't open serial port (name: %1, baud: %2)")
                   .arg(_settings->gpsSerialPortName->value)
                   .arg(_settings->gpsSerialBaudRate->value));

        delete _serialPort;
        _settings->Release();
        _settings = nullptr;
        return false;
    }

    _is_started = true;
    return true;
}

void GPSSensor::Stop()
{
    if(!_is_started)
    {
        LOG_WARNING("GPS Sensor arleady stoped");
        return;
    }

    LOG_INFO("Stoping GPS Sensor");
    if(_serialPort != nullptr)
    {
        _serialPort->close();
        delete _serialPort;
        _serialPort = nullptr;
    }

    if(_settings != nullptr)
    {
        _settings->Release();
        _settings = nullptr;
    }

    _is_started = false;

}

GPSData GPSSensor::Readings()
{
    QMutexLocker locker(&_mutex);
    GPSData return_data = _gps_data;
    return return_data;
}

void GPSSensor::onSerialData()
{
    while(_serialPort->canReadLine())
    {
        QByteArray line = _serialPort->readLine(256);
        parseData( line );
    }
}

void GPSSensor::parseData(QString line)
{
    QMutexLocker locker(&_mutex);
    if(_gps_data_parser.Parse(line))
    {
        _gps_data = _gps_data_parser.GpsData();
        emit onNewGpsData(_gps_data);
    }
}
