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

class FakeRealPrintGPIO : public GPIO
{
public:
    FakeRealPrintGPIO();
    ~FakeRealPrintGPIO();

    void EnablePwm(uchar n);
    void SetPinOuputMode(uchar n);

    void SetPin(uchar n, bool v);
    void SetPwm(uchar n, uchar v);
    bool Init();
};


}
#endif // GPIO_H
