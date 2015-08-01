#ifndef CLIENTASYNCWAITEVENT_H
#define CLIENTASYNCWAITEVENT_H

#include <QtCore>
#include <QSemaphore>

namespace mibot
{

class AsyncWaitEvent
{
public:
    AsyncWaitEvent(bool autoOpen);
    ~AsyncWaitEvent();

    bool Wait(uint ms);
    void Open();
    void Close();


private:
    QMutex _mutex;
    QSemaphore _sem;
    bool _autoOpen;
};


class AutoOpenAsyncWaitEvent : public AsyncWaitEvent
{
public:
    AutoOpenAsyncWaitEvent();
    ~AutoOpenAsyncWaitEvent();
};

class AutoCloseAsyncWaitEvent : public AsyncWaitEvent
{
public:
    AutoCloseAsyncWaitEvent();
    ~AutoCloseAsyncWaitEvent();
};

}
#endif // CLIENTASYNCWAITEVENT_H
