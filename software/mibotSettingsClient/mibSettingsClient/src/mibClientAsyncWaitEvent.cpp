#include <QMutexLocker>
#include "../inc/mibClientAsyncWaitEvent.h"

using namespace mibot;

ClientAsyncWaitEvent::ClientAsyncWaitEvent(bool autoOpen) :
    _sem(0),
    _autoOpen(autoOpen)
{}

ClientAsyncWaitEvent::~ClientAsyncWaitEvent()
{}

bool ClientAsyncWaitEvent::Wait(uint ms)
{
    if(!_sem.tryAcquire(1, ms))
        return false;

    if(_autoOpen) Open();

    return true;
}

void ClientAsyncWaitEvent::Close()
{
    QMutexLocker locker(&_mutex);
    if(_sem.available() == 1)
        _sem.acquire(1);
}

void ClientAsyncWaitEvent::Open()
{
    QMutexLocker locker(&_mutex);
    if(_sem.available() == 0)
        _sem.release(1);
}

ClientAutoOpenAsyncWaitEvent::ClientAutoOpenAsyncWaitEvent() :
    ClientAsyncWaitEvent(true)
{}

ClientAutoOpenAsyncWaitEvent::~ClientAutoOpenAsyncWaitEvent()
{}

ClientAutoCloseAsyncWaitEvent::ClientAutoCloseAsyncWaitEvent() :
    ClientAsyncWaitEvent(false)
{}

ClientAutoCloseAsyncWaitEvent::~ClientAutoCloseAsyncWaitEvent()
{}
