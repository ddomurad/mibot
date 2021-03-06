#ifndef DRIVESTRATEGY_H
#define DRIVESTRATEGY_H

#include <QtCore>
#include <mibGPIO.h>

#include "mibDriveSettings.h"
#include "mibDriveStrategyGlobal.h"

#include <mibAbstractSocketStrategy.h>
#include <mibReadWriteProtocol.h>
#include "mibDriveModel.h"

namespace mibot
{

class MIBDRIVESTRATEGYSHARED_EXPORT DriveStartegy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    explicit DriveStartegy( Connection * connection );
    ~DriveStartegy();

    // AbstractSocketStrategy interface
protected:
    void processNewData(QByteArray);
    bool init();

private slots:
    void onUpdate();

private:
    QTimer * _update_timer;
    QElapsedTimer _emergency_brake_timer;

    DriveConfigSettings *_driveSettings;
    DrivingState *_state;

    template <typename T>
    bool checkConfigs();
    static int _cnt;

    ReadWriteBytesProtocol _protocol_handler;

    GPIO *gpio();
    AbstractDriveModel * _model;

public:
    const uchar ADDRW_BRAKE   = 0x00;
    const uchar ADDRW_TURBO   = 0x01;
    const uchar ADDRW_DRIVE   = 0x02;
    const uchar ADDRW_TURN    = 0x03;

    const uchar ADDRR_IS_SIMULATION_MODE = 0x00;
    const uchar ADDRR_BRAKE = 0x01;
};

}

extern "C" mibot::AbstractSocketStrategy * createStrategy(mibot::Connection *connection);
#endif // DRIVESTRATEGY_H
