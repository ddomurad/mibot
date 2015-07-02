#ifndef SENSOR_H
#define SENSOR_H

#include <QtCore>
#include <QElapsedTimer>
#include <QMap>
#include "mibSensorsGlobal.h"

namespace mibot
{

template <class T>
class Sensor;

template <class T>
class MIBSENSORSSHARED_EXPORT SensorReader
{
public:
    SensorReader(float _T, Sensor<T> *sensor)
    {
        this->_T = _T;
        _sensor = sensor;
    }

    ~SensorReader()
    {}

    QVariant Read(T key)
    {
        return _sensor->ReadSensor(key, _T);
    }

    QMap<T,QVariant> ReadAll()
    {
        return _sensor->ReadAllSensors(_T);
    }

private:
    float _T;
    Sensor<T> * _sensor;
};

template <class T>
class MIBSENSORSSHARED_EXPORT Sensor
{
public:
    ~Sensor()
    {}

    bool  Initialize()
    {
        if(!_initialized)
        {
            _initialized = _intialize();
            _timer.start();
        }

        return _initialized;
    }

    QVariant ReadSensor(T key, float time)
    {
        updateReadsIfNeeded(time);
        return _last_reads[key];
    }

    QMap<T, QVariant> ReadAllSensors(float time)
    {
        updateReadsIfNeeded(time);
        return _last_reads;
    }

    SensorReader<T> *GetReader(float _T)
    {
        SensorReader<T> *reader = new SensorReader<T>(_T, this);
        return reader;
    }

protected:
    Sensor():
        _initialized(false)
    {}

    bool _initialized;
    void updateReadsIfNeeded(float time)
    {
        _mutex.lock();
        if(_timer.elapsed() > time)
        {
            _last_reads.clear();
            _readAllSensors();
            _timer.restart();
        }
        _mutex.unlock();
    }

    virtual bool _intialize() = 0;
    virtual void _readAllSensors() = 0;

    QMap<T,QVariant> _last_reads;
    QElapsedTimer _timer;
    QMutex _mutex;
};

}
#endif // SENSOR_H
