#ifndef SENSOR_H
#define SENSOR_H

#include <QtCore>
#include <QElapsedTimer>
#include <QMap>
#include "mibSensorsGlobal.h"

namespace mibot
{

template <class T, class Y>
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

    Y ReadSensor(T key)
    {
        _updateReadsIfNeeded();
        return getLastReads()[key];
    }

    QMap<T, Y> ReadAllSensors()
    {
        _updateReadsIfNeeded();
        return getLastReads();
    }

protected:
    Sensor():
        _initialized(false)
    {}

    bool _initialized;
    virtual void _updateReadsIfNeeded() = 0;
    virtual bool _intialize() = 0;

    virtual QMap<T,Y> getLastReads() = 0;
    QElapsedTimer _timer;
    QMutex _mutex;
};

}
#endif // SENSOR_H
