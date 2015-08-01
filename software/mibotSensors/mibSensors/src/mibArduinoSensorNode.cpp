#include <mibSettingsClient.h>
#include "inc/mibArduinoSensorNode.h"

using namespace mibot;

ArduinoSensorNode::ArduinoSensorNode()
{}

void ArduinoSensorNode::_updateReadsIfNeeded()
{

}

QMap<QString, float> ArduinoSensorNode::getLastReads()
{
    return _reader->Readings();
}

ArduinoSensorNode::~ArduinoSensorNode()
{}

ArduinoSensorNode *ArduinoSensorNode::get()
{
    static ArduinoSensorNode node;
    return &node;
}

bool ArduinoSensorNode::_intialize()
{
    _thread = new QThread();
    _reader = new ArduinoSensorsNodeReader();

    _reader->moveToThread( _thread );
    _thread->start();

    if(_reader->StartReader())
        return true;

    return false;
}
