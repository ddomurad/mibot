#include "inc/mibDriveSettings.h"

using namespace mibot;

DriveConfigSettings::DriveConfigSettings(QString res) :
    SettingsObject(res, true)
{
    alias = new StringSettingsValue("alias");
    useFakeGPIO = new BoolSettingsValue("use_fake_gpio");

    leftAPin = new IntSettingsValue("gpio_left_a");
    leftBPin = new IntSettingsValue("gpio_left_b");
    leftPwmPin = new IntSettingsValue("gpio_left_pwm");

    rightAPin = new IntSettingsValue("gpio_right_a");
    rightBPin = new IntSettingsValue("gpio_right_b");
    rightPwmPin = new IntSettingsValue("gpio_right_pwm");

    updateRatio = new IntSettingsValue("gpio_update_rate");
    emergencyBreakTimeout = new IntSettingsValue("emergency_break_timeout");

    AddValue( alias );
    AddValue( useFakeGPIO );

    AddValue( leftAPin );
    AddValue( leftBPin );
    AddValue( leftPwmPin );

    AddValue( rightAPin );
    AddValue( rightBPin );
    AddValue( rightPwmPin );

    AddValue( updateRatio );
    AddValue( emergencyBreakTimeout );
}

