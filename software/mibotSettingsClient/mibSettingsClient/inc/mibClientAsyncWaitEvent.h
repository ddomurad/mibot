#ifndef CLIENTASYNCWAITEVENT_H
#define CLIENTASYNCWAITEVENT_H

#include <QtCore>
#include <QSemaphore>

namespace mibot
{

class ClientAsyncWaitEvent
{
public:
    ClientAsyncWaitEvent(bool autoOpen);
    ~ClientAsyncWaitEvent();

    bool Wait(uint ms);
    void Open();
    void Close();


private:
    QMutex _mutex;
    QSemaphore _sem;
    bool _autoOpen;
};


class ClientAutoOpenAsyncWaitEvent : public ClientAsyncWaitEvent
{
public:
    ClientAutoOpenAsyncWaitEvent();
    ~ClientAutoOpenAsyncWaitEvent();
};

class ClientAutoCloseAsyncWaitEvent : public ClientAsyncWaitEvent
{
public:
    ClientAutoCloseAsyncWaitEvent();
    ~ClientAutoCloseAsyncWaitEvent();
};

}
#endif // CLIENTASYNCWAITEVENT_H
