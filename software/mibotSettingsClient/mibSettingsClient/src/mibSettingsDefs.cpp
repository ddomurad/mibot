#include "inc/mibSettingsDefs.h"

using namespace mibot;

PrivilegeSetting::PrivilegeSetting(QString res):
    SettingsObject(res,false)
{
    alias = new StringSettingsValue("alias");
    level = new IntSettingsValue("level");

    AddValue(alias);
    AddValue(level);
}

PrivilegeSetting::~PrivilegeSetting()
{}

QString PrivilegeSetting::DefaultDir = "./privileges/";

SocketSetting::SocketSetting(QString res):
    SettingsObject(res,false)
{
    alias = new StringSettingsValue("alias");
    privileges = new StringSettingsValue("privileges");
    port = new IntSettingsValue("port");
    strategy = new StringSettingsValue("strategy");
    strategyConfig = new StringSettingsValue("config");
    useSsl = new BoolSettingsValue("use_ssl");
    isEnabled = new BoolSettingsValue("enable");

    AddValue(alias);
    AddValue(privileges);
    AddValue(port);
    AddValue(strategy);
    AddValue(strategyConfig);
    AddValue(useSsl);
    AddValue(isEnabled);
}

SocketSetting::~SocketSetting()
{}

QString SocketSetting::DefaultDir = "./sockets/";

UserSetting::UserSetting(QString res):
    SettingsObject(res,false)
{
     privileges = new StringSettingsValue("privileges");
     alias = new StringSettingsValue("alias");
     certificate = new StringSettingsValue("certificate");
     isEnabled = new BoolSettingsValue("enabled");

     AddValue(privileges);
     AddValue(alias);
     AddValue(certificate);
     AddValue(isEnabled);
}

UserSetting::~UserSetting()
{}

QString UserSetting::DefaultDir = "./users/";
