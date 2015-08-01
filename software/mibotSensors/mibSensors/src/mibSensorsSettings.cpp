#include "inc/mibSensorsSettings.h"

using namespace mibot;

SensorsSettings *SensorsSettings::getGlobal()
{
    return SettingsClient::CreateReource<SensorsSettings>("./sensors/sensors.json");
}

SensorsSettings::SensorsSettings(QString res) :
    SettingsObject(res, true)
{
    systemSensorsUpdateTime = new IntSettingsValue("system_sensors_update_time");

    arduinoNodeSerialPortName = new StringSettingsValue("arduino_node_serial_port_name");
    arduinoNodeSerialBaudRate = new IntSettingsValue("arduino_node_serial_baud_rate");

    enable_acc = new IntSettingsValue("enable_acc");
    enable_mag = new IntSettingsValue("enable_mag");
    enable_gps = new IntSettingsValue("enable_gps");

    analog[0] = new IntSettingsValue("analog_1");
    analog[1] = new IntSettingsValue("analog_2");
    analog[2] = new IntSettingsValue("analog_3");
    analog[3] = new IntSettingsValue("analog_4");
    analog[4] = new IntSettingsValue("analog_5");

    AddValue( systemSensorsUpdateTime );

    AddValue( arduinoNodeSerialPortName );
    AddValue( arduinoNodeSerialBaudRate );

    AddValue(enable_acc);
    AddValue(enable_mag);
    AddValue(enable_gps);

    AddValue(analog[0]);
    AddValue(analog[1]);
    AddValue(analog[2]);
    AddValue(analog[3]);
    AddValue(analog[4]);
}

SensorsSettings::~SensorsSettings()
{}


