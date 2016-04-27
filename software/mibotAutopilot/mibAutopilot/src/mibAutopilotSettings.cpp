#include "inc/mibAutopilotSettings.h"

using namespace mibot;

AutopilotSettings::AutopilotSettings(QString res):
    SettingsObject(res,true)
{
    watchDogTimerInterval = new IntSettingsValue("watchdog_interval");
    driveSettings = new StringSettingsValue("drive_settings");
    maxDistance = new DoubleSettingsValue("max_dist");
    statuTimerInterval = new IntSettingsValue("status_interval");

    angleTreshold = new DoubleSettingsValue("angle_treshold");
    baseTimeFactor = new DoubleSettingsValue("base_time_factor");
    dynamicTimeFactor = new BoolSettingsValue("dynamic_time_factor");

    forwardDriveTime = new DoubleSettingsValue("forward_drive_time");

    AddValue(watchDogTimerInterval);
    AddValue(driveSettings);
    AddValue(maxDistance);
    AddValue(statuTimerInterval);

    AddValue(angleTreshold);
    AddValue(baseTimeFactor);
    AddValue(dynamicTimeFactor);
    AddValue(forwardDriveTime);
}

AutopilotSettings::~AutopilotSettings()
{}

