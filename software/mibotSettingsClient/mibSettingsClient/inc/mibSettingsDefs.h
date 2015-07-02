#ifndef SETTINGS_H
#define SETTINGS_H

#include "mibSettingsClient.h"
#include "mibSettingsObject.h"
#include "mibSettingsValue.h"

namespace mibot
{

class PrivilegeSetting : public SettingsObject
{
public:
    PrivilegeSetting(QString res);
    ~PrivilegeSetting();

    StringSettingsValue * alias;
    IntSettingsValue * level;

    static QString DefaultDir;
};

class SocketSetting : public SettingsObject
{
public:
    SocketSetting(QString res);
    ~SocketSetting();

    StringSettingsValue * alias;
    StringSettingsValue * privileges;
    IntSettingsValue * port;
    StringSettingsValue * strategy;
    StringSettingsValue * strategyConfig;
    BoolSettingsValue * useSsl;
    BoolSettingsValue * isEnabled;

    static QString DefaultDir;
};

class UserSetting : public SettingsObject
{
public:
    UserSetting(QString res);
    ~UserSetting();

    StringSettingsValue * privileges;
    StringSettingsValue * alias;
    StringSettingsValue * certificate;
    BoolSettingsValue * isEnabled;

    static QString DefaultDir;
};

}

#endif // SETTINGS_H
