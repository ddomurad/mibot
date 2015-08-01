#include <QMutexLocker>
#include "../inc/mibAsyncWaitEvent.h"

using namespace mibot;

AsyncWaitEvent::AsyncWaitEvent(bool autoOpen) :
    _sem(0),
    _autoOpen(autoOpen)
{}

AsyncWaitEvent::~AsyncWaitEvent()
{}

bool AsyncWaitEvent::Wait(uint ms)
{
    if(!_sem.tryAcquire(1, ms))
        return false;

    if(_autoOpen) Open();

    return true;
}

void AsyncWaitEvent::Close()
{
    QMutexLocker locker(&_mutex);
    if(_sem.available() == 1)
        _sem.acquire(1);
}

void AsyncWaitEvent::Open()
{
    QMutexLocker locker(&_mutex);
    if(_sem.available() == 0)
        _sem.release(1);
}

AutoOpenAsyncWaitEvent::AutoOpenAsyncWaitEvent() :
    AsyncWaitEvent(true)
{}

AutoOpenAsyncWaitEvent::~AutoOpenAsyncWaitEvent()
{}

AutoCloseAsyncWaitEvent::AutoCloseAsyncWaitEvent() :
    AsyncWaitEvent(false)
{}

AutoCloseAsyncWaitEvent::~AutoCloseAsyncWaitEvent()
{}
