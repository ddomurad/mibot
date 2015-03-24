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

void CarMotorController::Update(GPIO *gpio, ControllerStatusWrapper *status, float dt)
{
    qDebug() << "update car model ..." << dt;
}


TankMotorController::TankMotorController()
{}

TankMotorController::~TankMotorController()
{}

void TankMotorController::Update(GPIO *gpio, ControllerStatusWrapper *status, float dt)
{
    qDebug() << "update tank model ..." << dt;
}
