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
    void SetPiezzo(bool state);
    bool Initialize();

private:
    ArduinoSensorNode();
    ArduinoSensorsNodeReader * _reader;
    QThread * _thread;

    bool _isInitialized;
    QMutex _initMutex;
};

}

#endif // ARDUINOSENSORNODE_H
