#include <mibLogger.h>
#include "inc/mibIIRFilter.h"

using namespace mibot;

IIRFilterConfig *IIRFilterConfig::GetGlobal()
{
    return SettingsClient::CreateReource<IIRFilterConfig>("./sensors/iir_filter.json");
}

IIRFilterConfig::IIRFilterConfig(QString res) :
    SettingsObject(res, true)
{
    Order = new IntSettingsValue("order");

    A[0] = new DoubleSettingsValue( "a1" );
    A[1] = new DoubleSettingsValue( "a2" );
    A[2] = new DoubleSettingsValue( "a3" );
    A[3] = new DoubleSettingsValue( "a4" );

    B[0] = new DoubleSettingsValue( "b0" );
    B[1] = new DoubleSettingsValue( "b1" );
    B[2] = new DoubleSettingsValue( "b2" );
    B[3] = new DoubleSettingsValue( "b3" );
    B[4] = new DoubleSettingsValue( "b4" );

    AddValue( Order );

    AddValue( A[0] );
    AddValue( A[1] );
    AddValue( A[2] );
    AddValue( A[3] );

    AddValue( B[0] );
    AddValue( B[1] );
    AddValue( B[2] );
    AddValue( B[3] );
    AddValue( B[4] );
}

IIRFilterConfig::~IIRFilterConfig()
{}

IIRFilter::IIRFilter()
{
    X = new double[4];
    Y = new double[4];
    A = new double[4];
    B = new double[5];

    for(int i=0;i<4;i++)
        X[i] = Y[i] = 0;

    config = nullptr;
    _last_sync_cnt = -1;
}

IIRFilter::~IIRFilter()
{
    delete[] X;
    delete[] Y;
    delete[] A;
    delete[] B;

    if(config != nullptr)
        config->Release();
}

double IIRFilter::Filter(double x)
{
    SyncConfig();

    if(N == 0)
        return x;

    double y = x*B[0];
    for(int i=0;i<N;i++)
    {
        y += Y[i]*A[i]*-1;
        y += X[i]*B[i+1];
    }

    for(int i=N-1;i>0;i--)
    {
        Y[i] = Y[i-1];
        X[i] = X[i-1];
    }

    Y[0] = y;
    X[0] = x;

    return y;
}

void IIRFilter::SyncConfig()
{
    if(config == nullptr)
        config = IIRFilterConfig::GetGlobal();

    if(config->SyncCounter() == _last_sync_cnt)
        return;

    _last_sync_cnt = config->SyncCounter();
    N = config->Order->value;
    if(N != 0 && N != 2 && N != 4)
    {
        LOG_ERROR("Can't set iir filter ordet to: " + QString::number(N));
        N = 0;
        return;
    }

    for(int i=0;i<4;i++)
    {
        A[i] = config->A[i]->value;
        B[i] = config->B[i]->value;
    }

    B[4] = config->B[4]->value;
}
