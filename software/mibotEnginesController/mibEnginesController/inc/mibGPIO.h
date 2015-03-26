#ifndef GPIO_H
#define GPIO_H

#include <QtCore>

namespace mibot
{

class GPIO
{
public:
    GPIO();
    virtual ~GPIO();

    virtual void EnablePwm(uchar n) = 0;
    virtual void SetPinOuputMode(uchar n) = 0;

    virtual void SetPin(uchar n, bool v) = 0;
    virtual void SetPwm(uchar n, uchar v) = 0;

    virtual bool Init() = 0;
};


class FakePrintGPIO : public GPIO
{
public:
    FakePrintGPIO();
    ~FakePrintGPIO();

    void EnablePwm(uchar n);
    void SetPinOuputMode(uchar n);

    void SetPin(uchar n, bool v);
    void SetPwm(uchar n, uchar v);
    bool Init();
};

class RealGPIO : public GPIO
{
public:
    RealGPIO();
    ~RealGPIO();

    void EnablePwm(uchar n);
    void SetPinOuputMode(uchar n);

    void SetPin(uchar n, bool v);
    void SetPwm(uchar n, uchar v);
    bool Init();
};


class MappedGPIO
{
public :
    MappedGPIO(GPIO * gpio);
    ~MappedGPIO();

    static void MapLeftA(int p);
    static void MapLeftB(int p);
    static void MapRightA(int p);
    static void MapRightB(int p);
    static void MapLeftPWM(int p);
    static void MapRightPWM(int p);

    void SetLeft(bool a, bool b, int pwm);
    void SetRight(bool a, bool b, int pwm);

    GPIO * gpio;

private:
    static int la;
    static int lb;
    static int ra;
    static int rb;
    static int lpwm;
    static int rpwm;
};
}
#endif // GPIO_H
