#ifndef ARDUINOSENSORNODE_H
#define ARDUINOSENSORNODE_H

#include <QThread>

#include "mibSensorsSettings.h"
#include "mibArduinoSensorsNodeReader.h"

namespace mibot
{

class ArduinoSensorNode
{
public:
    ~ArduinoSensorNode();

    static ArduinoSensorNode * get();
    ArduinoReadings Readings();

    bool Initialize();
    void SetPiezo(bool state);

    bool AccCalibration(double new_acc_scale);
private:
    ArduinoSensorNode();
    ArduinoSensorsNodeReader * _reader;
    QThread * _thread;

    bool _isInitialized;
    QMutex _initMutex;
};

}

#endif // ARDUINOSENSORNODE_H
