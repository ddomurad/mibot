#ifndef MCP3008SENSOR_H
#define MCP3008SENSOR_H

#include "mibSensor.h"
#include <mibMCP3008.h>

namespace mibot
{

class MIBSENSORSSHARED_EXPORT MCP3008Sensor: public Sensor<int>
{
public:
    ~MCP3008Sensor();

    static MCP3008Sensor *get();
    // Sensor interface
protected:
    MCP3008Sensor();

    bool _intialize();
    void _readAllSensors();

private:
    MCP3008 mcp3008;
};

}

#endif // MCP3008SENSOR_H
