#ifndef STATUSCONFIGRES_H
#define STATUSCONFIGRES_H

#include <mibSettingsClient.h>

namespace mibot{

class StatusSettings : public SettingsObject
{
public:
    StatusSettings(QString res);
    ~StatusSettings();

    StringSettingsValue* alias;

    BoolSettingsValue    *useMcp3008;
    IntSettingsValue     *mcp3008Base;
    IntSettingsValue     *mcp3008SpiChannel;
    BoolSettingsValue    *readEnginesAccu;
    IntSettingsValue     *enginesAccuMcp3008Channel;
    DoubleSettingsValue  *enginesAccuVScale;
    BoolSettingsValue    *readCpuTemp;
    DoubleSettingsValue  *cpuTempScale;

    BoolSettingsValue    *readCpuUsage;
    BoolSettingsValue    *readMemory;

    IntSettingsValue     *updateRatio;
    IntSettingsValue     *internalDelay;
};

}

#endif // STATUSCONFIGRES_H
