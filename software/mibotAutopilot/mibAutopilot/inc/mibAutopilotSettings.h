#ifndef AUTOPILOTSETTINGS_H
#define AUTOPILOTSETTINGS_H

#include <mibSettingsClient.h>

namespace mibot{

class AutopilotSettings : public SettingsObject
{
public:
    AutopilotSettings(QString res);
    ~AutopilotSettings();

    IntSettingsValue * watchDogTimerInterval;
    IntSettingsValue * statuTimerInterval;
    DoubleSettingsValue * maxDistance;
    StringSettingsValue * driveSettings;
    DoubleSettingsValue * angleTreshold;

    IntSettingsValue * turnTime;
    IntSettingsValue * usTime;
};
}
#endif // AUTOPILOTSETTINGS_H
