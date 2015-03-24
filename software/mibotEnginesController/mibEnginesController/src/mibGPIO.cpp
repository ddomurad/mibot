#include "inc/mibGPIO.h"
#include <QDebug>

using namespace mibot;


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


FakeRealPrintGPIO::FakeRealPrintGPIO()
{

}

FakeRealPrintGPIO::~FakeRealPrintGPIO()
{

}

void FakeRealPrintGPIO::EnablePwm(uchar n)
{
    qDebug() << QString(" >> SET PIN TO PWM: %1 ").arg(n);
}

void FakeRealPrintGPIO::SetPinOuputMode(uchar n)
{
    qDebug() << QString(" >> SET PIN TO OUPUT: %1 ").arg(n);
}

void FakeRealPrintGPIO::SetPin(uchar n, bool v)
{
    qDebug() << QString(" >> SET PIN: %1 to %2").arg(n).arg(v);
}

void FakeRealPrintGPIO::SetPwm(uchar n, uchar v)
{
    qDebug() << QString(" >> SET PWM: %1 to %2").arg(n).arg(v);
}

bool FakeRealPrintGPIO::Init()
{
    return true;
}
