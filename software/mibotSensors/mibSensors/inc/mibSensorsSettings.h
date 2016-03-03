#ifndef SENSORSSETTINGS_H
#define SENSORSSETTINGS_H

#include <QtCore>
#include <mibSettingsClient.h>

namespace mibot
{

class SensorsSettings : public SettingsObject
{
public:

    static SensorsSettings * getGlobal();

    SensorsSettings(QString res);
    ~SensorsSettings();

    IntSettingsValue * systemSensorsUpdateTime;

    StringSettingsValue * arduinoNodeSerialPortName;
    IntSettingsValue * arduinoNodeSerialBaudRate;

    DoubleSettingsValue * analogFactor[10];
    IntSettingsValue * analog[10];
};

}
#endif // SENSORSSETTINGS_H
