#include "inc/mibGPIO.h"
#include <QDebug>

using namespace mibot;

extern "C"
{
    int wiringPiSetup (void);
    void pinMode (int pin, int mode);
    void digitalWrite (int pin, int value) ;
    int softPwmCreate (int pin, int initialValue, int pwmRange);
    void softPwmWrite(int pin, int value);
}

#ifndef ENABLE_GPIO

int wiringPiSetup (void)
{
    qDebug() << "wiringPiSetup ";
    return 0;
}
void pinMode (int pin, int mode)
{
    qDebug() << "pinMode " << pin << mode;
}
void digitalWrite (int pin, int value)
{
     qDebug() << "digitalWrite" << pin << value;
}
int softPwmCreate (int pin, int initialValue, int pwmRange)
{
    qDebug() << "softPwmCreate" << pin << initialValue <<pwmRange ;
    return 0;
}
void softPwmWrite(int pin, int value)
{
    qDebug() << "softPwmWrite" <<  pin << value;
}

#endif
GPIO::GPIO()
{}

GPIO::~GPIO()
{}


FakePrintGPIO::FakePrintGPIO()
{}

FakePrintGPIO::~FakePrintGPIO()
{}

void FakePrintGPIO::EnablePwm(uchar n)
{
    qDebug() << QString("SET PIN TO PWM: %1 ").arg(n);
}

void FakePrintGPIO::SetPinOuputMode(uchar n)
{
    qDebug() << QString("SET PIN TO OUPUT: %1 ").arg(n);
}

void FakePrintGPIO::SetPin(uchar n, bool v)
{
    qDebug() << QString("SET PIN: %1 to %2").arg(n).arg(v);
}

void FakePrintGPIO::SetPwm(uchar n, uchar v)
{
    qDebug() << QString("SET PWM: %1 to %2").arg(n).arg(v);
}

bool FakePrintGPIO::Init()
{

    return true;
}


RealGPIO::RealGPIO()
{

}

RealGPIO::~RealGPIO()
{

}

void RealGPIO::EnablePwm(uchar n)
{
    softPwmCreate(int(n),0, 256);
}

void RealGPIO::SetPinOuputMode(uchar n)
{
    pinMode (int(n), 1);
}

void RealGPIO::SetPin(uchar n, bool v)
{
    digitalWrite( int(n), v ? 1 : 0);
}

void RealGPIO::SetPwm(uchar n, uchar v)
{
    softPwmWrite( int(n), int(v));
}

bool RealGPIO::Init()
{
    wiringPiSetup();
    return true;
}


MappedGPIO::MappedGPIO(GPIO *g):
    gpio(g)
{

}

MappedGPIO::~MappedGPIO()
{
}

void MappedGPIO::MapLeftA(int p)
{ la = p; }

void MappedGPIO::MapLeftB(int p)
{ lb = p; }

void MappedGPIO::MapRightA(int p)
{ ra = p; }

void MappedGPIO::MapRightB(int p)
{ rb = p; }

void MappedGPIO::MapLeftPWM(int p)
{ lpwm = p; }

void MappedGPIO::MapRightPWM(int p)
{ rpwm = p; }

void MappedGPIO::SetLeft(bool a, bool b, int pwm)
{
    gpio->SetPin( la, a);
    gpio->SetPin( lb, b);
    gpio->SetPwm( lpwm, pwm);
}

void MappedGPIO::SetRight(bool a, bool b, int pwm)
{
    gpio->SetPin( ra, a);
    gpio->SetPin( rb, b);
    gpio->SetPwm( rpwm, pwm);
}

int MappedGPIO::la = 0;
int MappedGPIO::lb = 0;
int MappedGPIO::ra = 0;
int MappedGPIO::rb = 0;
int MappedGPIO::lpwm = 0;
int MappedGPIO::rpwm = 0;
