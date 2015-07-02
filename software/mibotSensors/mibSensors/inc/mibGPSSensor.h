#ifndef GPSSENSOR_H
#define GPSSENSOR_H

#include "mibSensor.h"

namespace mibot
{
class GPSSensor: public Sensor<QString>
{
public:
    ~GPSSensor();
    static GPSSensor * get();
private:
    GPSSensor();

    // Sensor interface
protected:
    bool _intialize();
    void _readAllSensors();
};
}

#endif // GPSSENSOR_H
