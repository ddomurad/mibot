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

    IntSettingsValue * analog[5];
    IntSettingsValue * enable_acc;
    IntSettingsValue * enable_mag;
    IntSettingsValue * enable_gps;
};

}
#endif // SENSORSSETTINGS_H
