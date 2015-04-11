#ifndef DRIVECONFIGRES_H
#define DRIVECONFIGRES_H

#include <mibAbstractResource.h>

namespace mibot
{

class DriveConfigRes : public AbstractResource
{
public:
    DriveConfigRes();
    ~DriveConfigRes();

    QString Alias();
    bool UseFakeGPIO();

    qint8 LeftAPin();
    qint8 LeftBPin();
    qint8 LeftPwmPin();

    qint8 RightAPin();
    qint8 RightBPin();
    qint8 RightPwmPin();

    qint16 UpdateRatio();
    qint16 EmergencyBreakTimeout();
private:
    qint16 _emergencyBreakTimeout;
    qint16 _updateRatio;
};

}
#endif // DRIVECONFIGRES_H
