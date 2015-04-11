#include "inc/mibDriveConfigRes.h"

using namespace mibot;

DriveConfigRes::DriveConfigRes() :
    AbstractResource("drive_strategy_cfg")
{
    _emergencyBreakTimeout = 0;
    _updateRatio = 0;

    AddField("alias",QVariant::String);
    AddField("use_fake_gpio",QVariant::Bool);
    AddField("gpio_left_a",QVariant::UInt);
    AddField("gpio_left_b",QVariant::UInt);
    AddField("gpio_left_pwm",QVariant::UInt);
    AddField("gpio_right_a",QVariant::UInt);
    AddField("gpio_right_b",QVariant::UInt);
    AddField("gpio_right_pwm",QVariant::UInt);
    AddField("gpio_update_ratio",QVariant::UInt);
    AddField("emergency_break_timeout",QVariant::UInt);
}

DriveConfigRes::~DriveConfigRes()
{}

QString DriveConfigRes::Alias()
{
    return Get("alias")->toString();
}

bool DriveConfigRes::UseFakeGPIO()
{
    return Get("use_fake_gpio")->toBool();
}

qint8 DriveConfigRes::LeftAPin()
{
    return Get("gpio_left_a")->toUInt();
}

qint8 DriveConfigRes::LeftBPin()
{
    return Get("gpio_left_b")->toUInt();
}

qint8 DriveConfigRes::LeftPwmPin()
{
    return Get("gpio_left_pwm")->toUInt();
}

qint8 DriveConfigRes::RightAPin()
{
    return Get("gpio_right_a")->toUInt();
}

qint8 DriveConfigRes::RightBPin()
{
    return Get("gpio_right_b")->toUInt();
}

qint8 DriveConfigRes::RightPwmPin()
{
    return Get("gpio_right_pwm")->toUInt();
}

qint16 DriveConfigRes::UpdateRatio()
{
    if(_updateRatio == 0)
        _updateRatio = Get("gpio_update_ratio")->toUInt();

    return _updateRatio;
}

qint16 DriveConfigRes::EmergencyBreakTimeout()
{
    if(_emergencyBreakTimeout == 0)
        _emergencyBreakTimeout = Get("emergency_break_timeout")->toUInt();

    return _emergencyBreakTimeout;
}

