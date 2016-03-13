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

    gpsSerialPortName = new StringSettingsValue("gps_serial_port_name");
    gpsSerialBaudRate = new IntSettingsValue("gps_serial_baud_rate");

    analogFactor[0] = new DoubleSettingsValue("analog_0_m");
    analogFactor[1] = new DoubleSettingsValue("analog_1_m");
    analogFactor[2] = new DoubleSettingsValue("analog_2_m");
    analogFactor[3] = new DoubleSettingsValue("analog_3_m");
    analogFactor[4] = new DoubleSettingsValue("analog_4_m");
    analogFactor[5] = new DoubleSettingsValue("analog_5_m");
    analogFactor[6] = new DoubleSettingsValue("analog_6_m");
    analogFactor[7] = new DoubleSettingsValue("analog_7_m");
    analogFactor[8] = new DoubleSettingsValue("analog_8_m");
    analogFactor[9] = new DoubleSettingsValue("analog_9_m");

    analog[0] = new IntSettingsValue("analog_0");
    analog[1] = new IntSettingsValue("analog_1");
    analog[2] = new IntSettingsValue("analog_2");
    analog[3] = new IntSettingsValue("analog_3");
    analog[4] = new IntSettingsValue("analog_4");
    analog[5] = new IntSettingsValue("analog_5");
    analog[6] = new IntSettingsValue("analog_6");
    analog[7] = new IntSettingsValue("analog_7");
    analog[8] = new IntSettingsValue("analog_8");
    analog[9] = new IntSettingsValue("analog_9");

    AddValue( systemSensorsUpdateTime );

    AddValue( arduinoNodeSerialPortName );
    AddValue( arduinoNodeSerialBaudRate );
    AddValue( gpsSerialPortName );
    AddValue( gpsSerialBaudRate );

    AddValue(analog[0]);
    AddValue(analog[1]);
    AddValue(analog[2]);
    AddValue(analog[3]);
    AddValue(analog[4]);
    AddValue(analog[5]);
    AddValue(analog[6]);
    AddValue(analog[7]);
    AddValue(analog[8]);
    AddValue(analog[9]);

    AddValue(analogFactor[0]);
    AddValue(analogFactor[1]);
    AddValue(analogFactor[2]);
    AddValue(analogFactor[3]);
    AddValue(analogFactor[4]);
    AddValue(analogFactor[5]);
    AddValue(analogFactor[6]);
    AddValue(analogFactor[7]);
    AddValue(analogFactor[8]);
    AddValue(analogFactor[9]);
}

SensorsSettings::~SensorsSettings()
{}


