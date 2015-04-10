#ifndef MCP3008_H
#define MCP3008_H

#include <QtCore>
#include "mibGPIOInterfaceGlobal.h"

namespace mibot
{

class MCP3008
{
public:
    MCP3008();
    ~MCP3008();

    bool Init(int pinBase, int channel);
    int ReadChannel(int channel);
private:
    int spi_channel;
    int pin_base;
};

}
#endif // MCP3008_H
