#include "inc/mibDriveModel.h"

using namespace mibot;

AbstarctWheelDriver::AbstarctWheelDriver()
{}

AbstarctWheelDriver::~AbstarctWheelDriver()
{}

float AbstarctWheelDriver::Speed()
{
    return speed;
}

void AbstarctWheelDriver::SetSpeed(float value)
{
    speed = value;
    Update();
}
float AbstarctWheelDriver::MaxSpeed()
{
    return max_speed;
}

void AbstarctWheelDriver::SetMaxSpeed(float value)
{
    max_speed = value;
    Update();
}
float AbstarctWheelDriver::Brake()
{
    return brake;
}

void AbstarctWheelDriver::SetBrake(float value)
{
    brake = value;
    Update();
}


WheelDriver::WheelDriver(qint8 a, qint8 b, qint8 pwm, GPIO * gpio)
{
    _pin_a = a;
    _pin_b = b;
    _pin_pwm = pwm;
    _gpio = gpio;
    _lsat_dir = 0;
}

WheelDriver::~WheelDriver()
{

}

void WheelDriver::Init()
{
    _gpio->SetPinMode( _pin_a, PinMode::Output );
    _gpio->SetPinMode( _pin_b, PinMode::Output );
    _gpio->SetPinMode( _pin_pwm, PinMode::Output );

    _gpio->EnablePwm( _pin_pwm, true );
    _gpio->SetPwmValue( _pin_pwm, 0 );
}

void WheelDriver::Update()
{
    if(speed > 1.0f) speed = 1.0f;
    if (speed < -1.0f) speed = -1.0f;

    if (max_speed > 1.0f) max_speed = 1.0f;
    if (max_speed < 0.0f) max_speed = 0.0f;

    if (!brake)
    {
        float p = fabs(speed * max_speed);
        int dir = (speed >= 0 ? 1 : -1);
        int pwmVal = (int)(100.0 * p);

        if (dir == 1)
        {
            _gpio->SetPin(_pin_a, true);
            _gpio->SetPin(_pin_b, false);
            _lsat_dir = 1;
        }
        else if (dir == -1)
        {
            _gpio->SetPin(_pin_a, false);
            _gpio->SetPin(_pin_b, true);
            _lsat_dir = -1;
        }

        _gpio->SetPwmValue( _pin_pwm, pwmVal );
    }
    else
    {
        _gpio->SetPwmValue( _pin_pwm, 0);

        if (_lsat_dir == 1)
        {
            _gpio->SetPin( _pin_a, false);
            _gpio->SetPin( _pin_b, true);
        }
        else if (_lsat_dir == -1)
        {
            _gpio->SetPin( _pin_a, true);
            _gpio->SetPin( _pin_b, false);
        }
    }
}


AbstractDriveModel::AbstractDriveModel() :
    _wheelLeft(nullptr),
    _wheelRight(nullptr),
    _state(nullptr)
{}


AbstractDriveModel::~AbstractDriveModel()
{
    if(_state != nullptr)       delete _state;
    if(_wheelLeft != nullptr)   delete _wheelLeft;
    if(_wheelRight != nullptr)  delete _wheelRight;
}

void AbstractDriveModel::Init(AbstarctWheelDriver *left, AbstarctWheelDriver *right, DrivingState *state)
{
    _wheelLeft  = left;
    _wheelRight = right;
    _state      = state;

    _wheelLeft->Init();
    _wheelRight->Init();

    _wheelLeft->SetMaxSpeed( 0.5f );
    _wheelRight->SetMaxSpeed( 0.5f );

    _wheelLeft->SetSpeed( 0.0f );
    _wheelRight->SetSpeed( 0.0f );

    _wheelLeft->SetBrake( false );
    _wheelRight->SetBrake( false );
}


VehicleDriveModel::VehicleDriveModel() :
    AbstractDriveModel(),
    _accelFactor(2.0f)
{}

VehicleDriveModel::~VehicleDriveModel()
{}

void VehicleDriveModel::Update(float dtime)
{
    if (_state->brake != 0)
    {
        _wheelLeft->SetBrake(true);
        _wheelRight->SetBrake(true);
        return;
    }

    if (_state->turbo != 0)
    {
        if (_wheelLeft->MaxSpeed() < 1.0f) _wheelLeft->SetMaxSpeed( _wheelLeft->MaxSpeed() + dtime);
        if (_wheelLeft->MaxSpeed() > 1.0f) _wheelLeft->SetMaxSpeed(1.0f);

        if (_wheelRight->MaxSpeed() < 1.0f) _wheelRight->SetMaxSpeed( _wheelRight->MaxSpeed() + dtime );
        if (_wheelRight->MaxSpeed() > 1.0f) _wheelRight->SetMaxSpeed( 1.0f );
    }
    else
    {
        _wheelLeft->SetMaxSpeed( 0.3f);
        _wheelRight->SetMaxSpeed( 0.3f );
        _accelFactor = 2.0f;
    }

    if (_state->drive_axis == 0)
    {
        if (_state->turn_axis != 0)
        {
            //float ta = float(_state->turn_axis) / 100.0f;
            _wheelLeft->SetSpeed( driveUp(_wheelLeft->Speed(), dtime * _accelFactor, (_state->turn_axis >= 0 ? 1 : -1)));
            _wheelRight->SetSpeed( driveUp(_wheelRight->Speed(), dtime * _accelFactor, (-_state->turn_axis >= 0 ? 1 : -1)));
        }
        else
        {
            _wheelLeft->SetSpeed( 0.0f);
            _wheelRight->SetSpeed( 0.0f);
        }
    }
    else
    {
        if (_state->turn_axis == 0)
        {
            _wheelLeft->SetSpeed( driveUp( _wheelLeft->Speed(), dtime * _accelFactor, (_state->drive_axis >= 0 ? 1 : -1)));
            _wheelRight->SetSpeed( driveUp( _wheelRight->Speed(), dtime * _accelFactor, (_state->drive_axis >= 0 ? 1 : -1)));
        }
        else
        {
            if (_state->turn_axis > 0.0f)
            {
                _wheelLeft->SetSpeed( driveUp( _wheelLeft->Speed(), dtime * _accelFactor, (_state->drive_axis >= 0 ? 1 : -1)));
                _wheelRight->SetSpeed( 0.0f );
            }
            else
            {
                _wheelRight->SetSpeed( driveUp( _wheelRight->Speed(), dtime * _accelFactor, (_state->drive_axis >= 0 ? 1 : -1)));
                _wheelLeft->SetSpeed( 0.0f );
            }
        }
    }

}

float VehicleDriveModel::driveUp(float val, float dtime, int sign)
{
    if(qAbs<float>(val) < 0.5f) return 0.5f * sign;
    return val + dtime * sign;
}

//float VehicleDriveModel::driveDown(float val, float dtime, int sign)
//{
//    if (qAbs<float>(val) - dtime > 0.0f)
//        return val - dtime * (_state->drive_axis >= 0 ? 1 : -1);
//    else
//        return 0;
//}


DrivingState::DrivingState()
{
    brake       = 0;
    turbo       = 0;
    drive_axis  = 0;
    turn_axis   = 0;
}

