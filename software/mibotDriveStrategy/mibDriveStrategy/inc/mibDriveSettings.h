#ifndef DRIVECONFIGRES_H
#define DRIVECONFIGRES_H

#include <mibSettingsObject.h>

namespace mibot
{

class DriveConfigSettings : public SettingsObject
{
public:
    DriveConfigSettings(QString res);
    ~DriveConfigSettings();

    StringSettingsValue *alias;
    BoolSettingsValue *useFakeGPIO;

    IntSettingsValue *leftAPin;
    IntSettingsValue *leftBPin;
    IntSettingsValue *leftPwmPin;

    IntSettingsValue *rightAPin;
    IntSettingsValue *rightBPin;
    IntSettingsValue *rightPwmPin;

    IntSettingsValue *updateRatio;
    IntSettingsValue *emergencyBreakTimeout;
};

}
#endif // DRIVECONFIGRES_H
