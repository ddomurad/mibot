#ifndef MIBAUTOPILOT_H
#define MIBAUTOPILOT_H

#include "mibAutopilotGlobal.h"
#include "mibAutopilotSettings.h"

#include <QObject>

#include <mibAbstractSocketStrategy.h>
#include <mibJSONProtocol.h>
#include <mibSensors.h>

#include <mibDriveModel.h>
#include <mibDriveSettings.h>

namespace mibot
{

class MIBAUTOPILOTSHARED_EXPORT Autopilot : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    Autopilot(Connection *connection);
    ~Autopilot();

    void SetupTest(AutopilotSettings *autopilotSettings, DriveConfigSettings *driveSettings, GPSSensor *gpsSensor);
    // AbstractSocketStrategy interface
protected:
    void processNewData(QByteArray data);
    bool init();

public slots:
    void onWatchDogTimer();
    void onDriverTimer();
    void onStateTimer();

    void onGpsData(GPSData);

private:
    JSONProtocol protocol;
    void tryRemoveCoruptedJSONProtocolData();
    void processCommand(QJsonObject &obj);

    GPSSensor * _gpsSensor;
    QTimer *_watch_dog_timer;
    QTimer *_driver_update_timer;
    QTimer *_state_update_timer;
    bool _watchdog_state;

    AutopilotSettings * _autopilotSettings;
    DriveConfigSettings * _driveSettings;

    GPIO *gpio();
    AbstractDriveModel * _model;
    DrivingState * _state;

    QPointF _target_location;
    int _target_id;

    void updateAutopilot();

    void setTarget(QPointF p, int id);

    void sendStatusMessage();
    bool isGpsSignalValid();
    bool _finished;

    QList<GPSData> _gps_data;

    enum AP_STATE {DISABLED = 0, DRIVE_FORWARD = 1, DRIVE_LEFT = 2, DRIVE_RIGHT = 3, WAIT_FOR_GPS_DATA = 4};
    AP_STATE _ap_state;
    bool _gps_data_recived;

    void enableAutopilot();
    void disableAutopilot();
    void updateMotors();
    void updateAutoPilotState();
    void updateTargetState();

    void setWaitForGpsState();
    void setTurningState();
    void setForwardState();

    qreal getDistace();
    qreal calcTurnData();
    qreal getRelativeAngleTo(qreal ar, QPointF gp);

    void setMotors(bool left, bool right);

    qreal _time_factor;
    qreal _angleToTurn;
    QElapsedTimer _state_timer;
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);
#endif // MIBAUTOPILOT_H
