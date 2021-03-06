#include <mibLogger.h>
#include <mibSettingsClient.h>
#include "inc/mibArduinoSensorNode.h"

using namespace mibot;

ArduinoSensorNode::ArduinoSensorNode()
{
    _isInitialized = false;
}

ArduinoReadings ArduinoSensorNode::Readings()
{
    return _reader->Readings();
}

void ArduinoSensorNode::SetPiezo(bool state)
{
    emit _reader->SetPiezo(state);
}

bool ArduinoSensorNode::AccCalibration(double new_acc_scale)
{
    return _reader->CalibrateAcc(new_acc_scale);
}

ArduinoSensorNode::~ArduinoSensorNode()
{}

ArduinoSensorNode *ArduinoSensorNode::get()
{
    static ArduinoSensorNode node;
    return &node;
}

bool ArduinoSensorNode::Initialize()
{
    QMutexLocker locket(&_initMutex);

    if(_isInitialized == true)
        return true;

    _thread = new QThread();
    _reader = new ArduinoSensorsNodeReader();

    _reader->moveToThread( _thread );
    _thread->start();

    if(_reader->StartReader())
    {
        _isInitialized = true;
        return true;
    }

    _thread->quit();

    if(!_thread->wait(3000))
    {
        LOG_WARNING("Can't join arduino read thread after init failure.");
    }

    return false;
}
