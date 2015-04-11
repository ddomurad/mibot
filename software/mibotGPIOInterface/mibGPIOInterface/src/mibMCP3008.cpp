#include "inc/mibMCP3008.h"

using namespace mibot;

extern "C"
{
    int mcp3004Setup (int, int);
    int analogRead (int) ;
}

#ifndef ENABLE_RPI_GPIO
    int mcp3004Setup (int, int)
    {
        return 0;
    }

    int analogRead (int)
    {
        float ce = float(QDateTime::currentDateTime().time().msec() + QDateTime::currentDateTime().time().second()*1000);
        return int ( 1000 * sin( 2.0f*3.1415f* ce / 5000.0f )) + 1000;
    }

#endif

MCP3008::MCP3008()
{}

MCP3008::~MCP3008()
{}

bool MCP3008::Init(int pinBase, int channel)
{
    if(mcp3004Setup(pinBase,channel) == -1)
        return false;

    spi_channel = channel;
    pin_base = pinBase;

    return true;
}

int MCP3008::ReadChannel(int channel)
{
    return analogRead( pin_base + channel );
}


