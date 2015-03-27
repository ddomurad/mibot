#ifndef DRIVEMODEL_H
#define DRIVEMODEL_H

#include <QtCore>
#include <mibGPIO.h>

namespace mibot
{

class AbstarctWheelDriver
{
public:
    AbstarctWheelDriver();
    virtual ~AbstarctWheelDriver();

    virtual void Init() = 0;

    float Speed();
    void SetSpeed(float value);

    float MaxSpeed();
    void SetMaxSpeed(float value);

    float Brake();
    void SetBrake(float value);

protected:
    virtual void Update() = 0;

    float speed;
    float max_speed;
    float brake;

};

class WheelDriver : public AbstarctWheelDriver
{
public:
    WheelDriver(qint8 pin_a, qint8 pin_b, qint8 pin_pwm, GPIO *gpio);
    ~WheelDriver();

    void Init();
protected:
    void Update();

private:
    qint8 _pin_a;
    qint8 _pin_b;
    qint8 _pin_pwm;
    GPIO *_gpio;

    qint8 _lsat_dir;
};

class DrivingState
{
public:
    DrivingState();

    qint8 brake;
    qint8 turbo;
    qint8 drive_axis;
    qint8 turn_axis;
};

class AbstractDriveModel
{
public:
    AbstractDriveModel();
    virtual ~AbstractDriveModel();

    void Init(AbstarctWheelDriver * left, AbstarctWheelDriver * right, DrivingState * state);
    virtual void Update(float dtime) = 0;

protected:

    AbstarctWheelDriver * _wheelLeft;
    AbstarctWheelDriver * _wheelRight;
    DrivingState        * _state;
};

class VehicleDriveModel : public AbstractDriveModel
{
public:
    VehicleDriveModel();
    ~VehicleDriveModel();

    // AbstractDriveModel interface
    void Update(float dtime);

private:
    float driveUp(float val, float dtime, int sign);
//    float driveDown(float val, float dtime, int sign);

    float _accelFactor;
};

}
#endif // DRIVEMODEL_H
