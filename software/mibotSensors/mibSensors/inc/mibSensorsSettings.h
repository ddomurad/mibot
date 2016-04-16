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

    StringSettingsValue * gpsSerialPortName;
    IntSettingsValue * gpsSerialBaudRate;

    IntSettingsValue * accXAxisIndex;
    IntSettingsValue * accYAxisIndex;
    IntSettingsValue * accZAxisIndex;

    IntSettingsValue * magXAxisIndex;
    IntSettingsValue * magYAxisIndex;
    IntSettingsValue * magZAxisIndex;

    DoubleSettingsValue * accScale;
};

}
#endif // SENSORSSETTINGS_H
