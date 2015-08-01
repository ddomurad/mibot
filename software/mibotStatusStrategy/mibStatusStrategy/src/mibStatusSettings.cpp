#include "inc/mibStatusSettings.h"

using namespace mibot;

StatusSettings::StatusSettings(QString res) :
    SettingsObject(res,true)
{
    alias               = new StringSettingsValue("alias");
    updateRatio         = new IntSettingsValue("values_update_ratio");
    internalDelay       = new IntSettingsValue("internal_delay_value");


    AddValue(alias);
    AddValue(updateRatio);
    AddValue(internalDelay);
}

StatusSettings::~StatusSettings()
{}
