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
    turnTime = new IntSettingsValue("turn_time");

    AddValue(watchDogTimerInterval);
    AddValue(driveSettings);
    AddValue(maxDistance);
    AddValue(statuTimerInterval);

    AddValue(angleTreshold);
    AddValue(turnTime);
}

AutopilotSettings::~AutopilotSettings()
{}

