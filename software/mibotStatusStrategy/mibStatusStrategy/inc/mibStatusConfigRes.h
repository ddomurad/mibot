#ifndef STATUSCONFIGRES_H
#define STATUSCONFIGRES_H

#include <mibAccessResources.h>

namespace mibot{

class StatusConfigRes : public AbstractResource
{
public:
    StatusConfigRes();
    ~StatusConfigRes();

    QString Alias();
    bool    UseMcp3008();
    int     Mcp3008Base();
    int     Mcp3008SpiChannel();
    bool    ReadEnginesAccu();
    int     EnginesAccuMcp3008Channel();
    double  EnginesAccuVScale();
    bool    ReadCpuTemp();

    int     UpdateRatio();
    int     InternalDelay();
};

}

#endif // STATUSCONFIGRES_H
