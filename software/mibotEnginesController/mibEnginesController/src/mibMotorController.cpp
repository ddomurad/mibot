#include "inc/mibMotorController.h"
#include <QDebug>

using namespace mibot;

MotorController::MotorController()
{}

MotorController::~MotorController()
{}


CarMotorController::CarMotorController()
{}

CarMotorController::~CarMotorController()
{}

void CarMotorController::Update(MappedGPIO *gpio, ControllerStatusWrapper *status, float dt)
{
    qDebug() << "update car model ..." << dt;
}


TankMotorController::TankMotorController()
{}

TankMotorController::~TankMotorController()
{}

void TankMotorController::Update(MappedGPIO *gpio, ControllerStatusWrapper *status, float)
{
    if(status->BrakeOn() || status->EmergencyBrakeOn())
    {
        gpio->SetLeft(false, false, 0);
        gpio->SetRight(false, false, 0);
        return;
    }

    int left_pwm = 255.0 * fabs(status->LeftMotors());
    int right_pwm = 255.0 * fabs(status->RightMotors());

    int left_sign = status->LeftMotors() > 0 ? 1 : -1;
    int right_sign = status->RightMotors() > 0 ? 1 : -1;

    if(left_pwm != 0)
        gpio->SetLeft( left_sign > 0, left_sign < 1, left_pwm  );
    else
        gpio->SetLeft( false, false, 0);

    if(right_pwm != 0)
        gpio->SetRight( right_sign > 0, right_sign < 1, right_pwm  );
    else
        gpio->SetRight( false, false, 0);
}
