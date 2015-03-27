#ifndef DRIVESTRATEGY_H
#define DRIVESTRATEGY_H

#include <QtCore>
#include <mibGPIO.h>

#include "mibAbstractSocketStrategy.h"
#include "mibReadWriteProtocol.h"

namespace mibot
{

class DriveStartegyConfig
{
public:
    DriveStartegyConfig();
    // pins
    qint8 gpio_left_a;
    qint8 gpio_left_b;
    qint8 gpio_left_pwm;

    qint8 gpio_right_a;
    qint8 gpio_right_b;
    qint8 gpio_right_pwm;

    // system
    qint8 gpio_update_ratio;
    qint8 emergency_break_timeout;
    qint8 fake_gpio;
};

class DriverStartegyState
{
public:
    DriverStartegyState();

    qint8 brake;
    qint8 emergency_brake;
    qint8 drive_model;

    qint8 left_motors_speed;
    qint8 right_motors_speed;
};

class DriveStartegy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    explicit DriveStartegy( Connection * connection );
    ~DriveStartegy();

    // AbstractSocketStrategy interface
protected:
    void processNewData(QByteArray);
    bool init();

private slots:
    void onUpdate();

private:
    QTimer * _update_timer;
    QElapsedTimer _emergency_brake_timer;

    DriveStartegyConfig _config;
    DriverStartegyState _state;

    bool getValue(qint8 *val, GlobalConfigRes * res);
    bool checkConfigs();
    static int _cnt;

    ReadWriteBytesProtocol _protocol_handler;

    mibot::GPIO *gpio();
private:

public:
    const uchar ADDRW_DRIVE_MODEL       = 0x00;
    const uchar ADDRW_BRAKE             = 0x01;
    const uchar ADDRW_LEFT_MOTORS_SPEED   = 0x02;
    const uchar ADDRW_RIGHT_MOTORS_SPEED  = 0x03;

    const uchar ADDRR_IS_SIMULATION_MODE = 0x00;
    const uchar ADDRR_EMERGENCY_BRAKE = 0x01;
};

}

#endif // DRIVESTRATEGY_H
