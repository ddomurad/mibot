#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <QtCore>
#include "mibGPIO.h"
#include "mibControllerStatus.h"

namespace mibot
{

class MotorController
{
public:
    MotorController();
    virtual ~MotorController();

    virtual void Update(MappedGPIO * gpio, ControllerStatusWrapper * status, float dt) = 0;
};

class CarMotorController : public MotorController
{
public:
    CarMotorController();
    ~CarMotorController();

    void Update(MappedGPIO * gpio, ControllerStatusWrapper * status, float dt);
};

class TankMotorController : public MotorController
{
public:
    TankMotorController();
    ~TankMotorController();

    void Update(MappedGPIO * gpio, ControllerStatusWrapper * status, float);
};

}

#endif // MOTORCONTROLLER_H
