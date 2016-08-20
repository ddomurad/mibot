#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QtCore>

class AppSettings
{
public:
    static QString MIBOT_Addr;
    static QString MIBOT_Video_Info_Port;

    static QString OSM_Server_Address;
    static QString OSM_Missing_Tile;
    static QString OSM_Cache_Dir;
    static QString OSM_Tiles_Format;

    static QString MapEdit_Locations_File;
    static QString MapEdit_Routes_File;
    static QString MapEdit_Marker_Color;
    static QString MapEdit_Active_Marker_Color;
    static QString MapEdit_Selected_Route_Color;
    static QString MapEdit_Enabled_Route_Opacity;
    static QString MapEdit_Disabled_Route_Opacity;
    static QString MapEdit_Start_Location_Name;

    static QString Connection_Rover_Addr;
    static QString Connection_Rover_Info_Port;
    static QString Connection_Rover_Info_SSL;

    static QString Connection_Cert_Path;
    static QString Connection_PK_Path;
    static QString Connection_CA_Path;

    static QString Connection_Status_Service;
    static QString Connection_Driver_Service;
    static QString Connection_Video_Service;
    static QString Connection_Picture_Service;
    static QString Connection_Autopilot_Service;

    static QString VServer_Listen_Port;
    static QString VServer_Forward_Addr;
    static QString VServer_Forward_Port;

    static QString JS_Drive_Axis;
    static QString JS_Turn_Axis;
    static QString JS_Brake_Btn;
    static QString JS_Turbo_Btn;
    static QString JS_Horn_Btn;
    static QString JS_Device_Path;
    static QString JS_Send_Ms;

    static QString JS_Swap_Drive;
    static QString JS_Swap_Trun;

    static QString Recording_Store_Dir;

    ~AppSettings();
    static AppSettings * get();

    static QVariant GetKey(QString key);
    static void SetKey(QString key, QVariant value);

    static void SaveAllCachedSettings();
    static void ClearCachedSettings();

private:
    AppSettings();

    static QMap<QString, QVariant> _settings;

    static QVariant findKey(QString key, bool *found);

    static QVariant loadKeyFromFile(QString key);
    static void    saveKeyToFile(QString key, QVariant value);
};


#endif // APPSETTINGS_H
