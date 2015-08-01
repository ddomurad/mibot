#ifndef ARDUINOSENSORNODE_H
#define ARDUINOSENSORNODE_H

#include <QThread>

#include "mibSensorsSettings.h"
#include "mibSensor.h"
#include "mibArduinoSensorsNodeReader.h"

namespace mibot
{

class ArduinoSensorNode : public Sensor<QString, float>
{
public:
    ~ArduinoSensorNode();

    static ArduinoSensorNode * get();
    // Sensor interface
protected:
    bool _intialize();
    void _readAllSensors();

private:
    ArduinoSensorNode();

    ArduinoSensorsNodeReader * _reader;
    QThread * _thread;

    // Sensor interface
protected:
    void _updateReadsIfNeeded();
    QMap<QString, float> getLastReads();
};

}

#endif // ARDUINOSENSORNODE_H
