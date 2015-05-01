#include <QDebug>
#include "inc/mibGPIO.h"

extern "C"
{
int     wiringPiSetup   (void );
void    pinMode         ( int pin, int mode     );
void    digitalWrite    ( int pin, int value    );
int     softPwmCreate   ( int pin, int initialValue, int pwmRange   );
void    softPwmWrite    ( int pin, int value    );
void    softPwmStop     ( int pin );
}

#ifndef ENABLE_RPI_GPIO
int wiringPiSetup(void )
{
    qDebug() << "(GPIO DISABLED) wiringPiSetup";
    return 0;
}

void pinMode( int , int )
{
    qDebug() << "(GPIO DISABLED) pinMode" << pin << mode;
}

void digitalWrite( int , int )
{
    //qDebug() << "digitalWrite" << pin << value;
}

int softPwmCreate( int , int , int )
{
    qDebug() << "(GPIO DISABLED) softPwmCreate" << pin << initialValue << pwmRange;

    return 0;
}

void softPwmWrite( int , int )
{
    //qDebug() << "softPwmWrite" << pin << value;
}

void softPwmStop(int )
{
    qDebug() << "(GPIO DISABLED) softPwmStop" << pin;
}

#endif

using namespace mibot;

GPIO::~GPIO()
{}

GPIO *GPIO::GetGPIO(bool real_rpi)
{
    static RpiGPIO rpi_gpio;
    static SimulatedGPIO fake_gpio;

    if(real_rpi)
        return & rpi_gpio;
    else
        return & fake_gpio;
}


// real gpio
RpiGPIO::RpiGPIO()
{
    _is_inited = false;
}

RpiGPIO::~RpiGPIO()
{}

bool RpiGPIO::Init()
{
    if(_is_inited) return true;

    if(wiringPiSetup() == 0 )
    {
        _is_inited = true;
        return true;
    }

    return false;
}

void RpiGPIO::SetPinMode(int pin, PinMode mode)
{
    pinMode( pin, int(mode) );
}

void RpiGPIO::SetPin(int pin, bool val)
{
    digitalWrite( pin, int(val) );
}

bool RpiGPIO::EnablePwm(int pin, bool enable)
{
    if(enable == true)
    {
        if( _enabled_pwms.contains(pin) )
            return true;

        if(softPwmCreate( pin, 0, 100) == 0)
        {
            _enabled_pwms.append( pin );
            return true;
        }
        else
            return false;
    }
    else
    {
        if( !_enabled_pwms.contains(pin) ) return true;
        _enabled_pwms.removeAll( pin );
        softPwmStop( pin );
    }

    return true;
}

void RpiGPIO::SetPwmValue(int pin, int val)
{
    if(val < 0) val = 0;
    if(val > 100) val = 100;
    softPwmWrite( pin, val );
}

void RpiGPIO::DisableAllPwms()
{
    for( int pin : _enabled_pwms )
        softPwmStop( pin );
    _enabled_pwms.clear();
}

bool SimulatedGPIO::Init()
{
    qDebug() << "SimulatedGPIO::Init()";
    return true;
}

void SimulatedGPIO::SetPinMode(int pin, PinMode mode)
{
    qDebug() << QString("SimulatedGPIO::SetPinMode(%1, %2).").arg(pin).arg(int(mode));
}

void SimulatedGPIO::SetPin(int pin, bool val)
{
    qDebug() << QString("SimulatedGPIO::SetPin(%1, %2).").arg(pin).arg(val);
}

bool SimulatedGPIO::EnablePwm(int pin, bool enable)
{
    qDebug() << QString("SimulatedGPIO::EnablePwm(%1, %2).").arg(pin).arg(enable);
    return true;
}

void SimulatedGPIO::SetPwmValue(int pin, int val)
{
    qDebug() << QString("SimulatedGPIO::SetPwmValue(%1, %2).").arg(pin).arg(val);
}

void SimulatedGPIO::DisableAllPwms()
{
    qDebug() << QString("SimulatedGPIO::DisableAllPwms()");
}
