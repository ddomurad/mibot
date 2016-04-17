#ifndef IIRFILTER_H
#define IIRFILTER_H

#include <mibSettingsClient.h>
#include <mibSettingsObject.h>

namespace mibot
{

class IIRFilterConfig : public SettingsObject
{
public:
    static IIRFilterConfig * GetGlobal();

    IIRFilterConfig(QString res);
    ~IIRFilterConfig();

    IntSettingsValue * Order;
    DoubleSettingsValue * A[4];
    DoubleSettingsValue * B[5];
};

class IIRFilter
{
public:
    IIRFilter();
    ~IIRFilter();

    double Filter(double x);
    void SyncConfig();
private:
    double *X;
    double *Y;

    double *A;
    double *B;

    uint N;
    uint _last_sync_cnt;
    IIRFilterConfig * config;
};

}

#endif // IIRFILTER_H
