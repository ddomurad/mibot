#include "inc/mibMCP3008Sensor.h"

using namespace mibot;

MCP3008Sensor::MCP3008Sensor()
{}

MCP3008Sensor::~MCP3008Sensor()
{}

MCP3008Sensor *MCP3008Sensor::get()
{
    static MCP3008Sensor sensor;
    return &sensor;
}

bool MCP3008Sensor::_intialize()
{
    return mcp3008.Init(100,0);
}

void MCP3008Sensor::_readAllSensors()
{
    for(int i=0; i<8; i++)
        _last_reads.insert( i, QVariant(mcp3008.ReadChannel(i)) );

}

