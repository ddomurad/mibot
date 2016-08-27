#include "AppSettings.h"

AppSettings::AppSettings()
{}

QVariant AppSettings::findKey(QString key, bool *found)
{
    if(!_settings.contains(key))
    {
        *found = false;
        return QVariant();
    }

    *found = true;
    return _settings[key];
}

QVariant AppSettings::loadKeyFromFile(QString combinedKey)
{
    QSettings settings("./config.cfg",QSettings::NativeFormat);
    return settings.value(combinedKey);
}

void AppSettings::saveKeyToFile(QString key, QVariant value)
{
    QSettings settings("./config.cfg",QSettings::NativeFormat);
    settings.setValue(key, value);
}

AppSettings::~AppSettings()
{}

AppSettings *AppSettings::get()
{
    static AppSettings settings;
    return &settings;
}

QVariant AppSettings::GetKey(QString key)
{
    bool found = false;
    QVariant value = findKey(key, &found);
    if(!found)
    {
        value = loadKeyFromFile(key);
        SetKey(key, value);
    }
    return value;
}

void AppSettings::SetKey(QString key, QVariant value)
{
    if(!_settings.contains(key))
        _settings.insert(key, value);
    else
        _settings[key] = value;
}

void AppSettings::SaveAllCachedSettings()
{
    QList<QString> keys = _settings.keys();
    for(int i=0; i< keys.size(); i++)
    {
        saveKeyToFile(keys[i], _settings[keys[i]]);
    }
}

void AppSettings::ClearCachedSettings()
{
    _settings.clear();
}


QMap<QString, QVariant> AppSettings::_settings = QMap<QString, QVariant>();

QString AppSettings::OSM_Server_Address = "OSM_server_address";
QString AppSettings::OSM_Missing_Tile = "OSM_missing_tile";
QString AppSettings::OSM_Cache_Dir = "OSM_cache_dir";
QString AppSettings::OSM_Tiles_Format = "OSM_tiles_format";

QString AppSettings::MapEdit_Locations_File = "MapEdit_location_file";
QString AppSettings::MapEdit_Routes_File = "MapEdit_routes_file";
QString AppSettings::MapEdit_Marker_Color = "MapEdit_marker_color";
QString AppSettings::MapEdit_Active_Marker_Color = "MapEdit_active_marker_color";
QString AppSettings::MapEdit_Selected_Route_Color = "MapEdit_selected_route_color";
QString AppSettings::MapEdit_Enabled_Route_Opacity = "MapEdit_enabled_route_opacity";
QString AppSettings::MapEdit_Disabled_Route_Opacity = "MapEdit_disabled_route_opacity";
QString AppSettings::MapEdit_Start_Location_Name = "MapEdit_start_location_nane";


QString AppSettings::Connection_Rover_Addr = "Connection_rover_addr";
QString AppSettings::Connection_Rover_Info_Port = "Connection_rover_info_port";
QString AppSettings::Connection_Rover_Info_SSL = "Connection_rover_info_ssl";
QString AppSettings::Connection_Cert_Path = "Connection_cert_path";
QString AppSettings::Connection_PK_Path = "Connection_pk_path";
QString AppSettings::Connection_CA_Path = "Connection_ca_path";

QString AppSettings::Connection_Status_Service = "Connection_status_service";
QString AppSettings::Connection_Driver_Service = "Connection_driver_service";
QString AppSettings::Connection_Video_Service = "Connection_video_service";
QString AppSettings::Connection_Picture_Service = "Connection_Picture_Service";
QString AppSettings::Connection_Autopilot_Service = "Connection_Autopilot_Service";

QString AppSettings::JS_Drive_Axis = "JS_Drive_Axis";
QString AppSettings::JS_Turn_Axis = "JS_Turn_Axis";
QString AppSettings::JS_Brake_Btn = "JS_Brake_Btn";
QString AppSettings::JS_Turbo_Btn = "JS_Turbo_Btn";
QString AppSettings::JS_Horn_Btn = "JS_Horn_Btn";
QString AppSettings::JS_Device_Path = "JS_Device_Path";
QString AppSettings::JS_Send_Ms = "JS_Send_Ms";
QString AppSettings::JS_Swap_Drive = "JS_Swap_Drive";
QString AppSettings::JS_Swap_Trun = "JS_Swap_Turn";

QString AppSettings::Recording_Store_Dir = "Recording_Store_Dir";
QString AppSettings::Picture_Store_Dir = "Picture_Store_Dir";
