#ifndef MIBGPIO_H
#define MIBGPIO_H

#include <QList>
#include "mibGPIOInterfaceGlobal.h"

namespace mibot
{

enum class PinMode { Input = 0 , Output = 1};
class MIBGPIOINTERFACESHARED_EXPORT GPIO
{
public:
    virtual ~GPIO();
    virtual bool Init() = 0;
    virtual void SetPinMode( int pin, PinMode mode) = 0;
    virtual void SetPin( int pin, bool val ) = 0;
    virtual bool EnablePwm( int pin, bool enable) = 0;
    virtual void SetPwmValue( int pin, int val) = 0;

    virtual void DisableAllPwms() = 0;

    static GPIO * GetGPIO(bool real_rpi);
};

class MIBGPIOINTERFACESHARED_EXPORT RpiGPIO : public GPIO
{
public:
    RpiGPIO();
    ~RpiGPIO();

    bool Init();
    void SetPinMode( int pin, PinMode mode);
    void SetPin( int pin, bool val );
    bool EnablePwm( int pin, bool enable);
    void SetPwmValue(int pin, int val);

    void DisableAllPwms();

private:
    bool _is_inited;
    QList<int> _enabled_pwms;
};



class MIBGPIOINTERFACESHARED_EXPORT SimulatedGPIO: public GPIO
{
public:
    bool Init();
    void SetPinMode( int pin, PinMode mode);
    void SetPin( int pin, bool val );
    bool EnablePwm( int pin, bool enable);
    void SetPwmValue( int pin, int val);

    void DisableAllPwms();
};

}



#endif // MIBGPIO_H
