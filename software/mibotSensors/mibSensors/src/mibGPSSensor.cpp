#include "inc/mibGPSSensor.h"

using namespace mibot;

GPSSensor::GPSSensor()
{}

bool GPSSensor::_intialize()
{
    return true;
}

void GPSSensor::_readAllSensors()
{
    static float a = 0;
    a += 3.1415 / 10.0;
    float x = cos(a);
    float y = sin(a);

    _last_reads.insert("pos", QVariant(QPointF(19.959 +x, 50.060 +y)));
}

GPSSensor::~GPSSensor()
{}

GPSSensor *GPSSensor::get()
{
    static GPSSensor sensor;
    return &sensor;
}

