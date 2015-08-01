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
    IntSettingsValue     *updateRatio;
    IntSettingsValue     *internalDelay;
};

}

#endif // STATUSCONFIGRES_H
