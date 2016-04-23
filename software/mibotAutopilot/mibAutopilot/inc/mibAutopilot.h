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

    // AbstractSocketStrategy interface
protected:
    void processNewData(QByteArray data);
    bool init();

private slots:
    void onWatchDogTimer();
    void onDriverTimer();
    void onStateTimer();

private:
    JSONProtocol protocol;
    void tryRemoveCoruptedJSONProtocolData();
    void processCommand(QJsonObject &obj);

    GPSSensor * _gpsSensor;
    QTimer *_watch_dog_timer;
    QTimer *_driver_update_timer;
    QTimer *_state_timer;
    bool _watchdog_state;

    AutopilotSettings * _autopilotSettings;
    DriveConfigSettings * _driveSettings;

    GPIO *gpio();
    AbstractDriveModel * _model;
    DrivingState * _state;

    QElapsedTimer _driver_update_stopwatch;

    qreal getDistace();
    qreal getRelativeAngle();
    QPointF gpsPosition();
    qreal gpsCource();

    QPointF _target_location;
    int _target_id;
    void updateAutopilot();

    void setTarget(QPointF p, int id);

    bool _autopilot_enabled;
    void enableAutopilot();
    void disableAutopilot();

    void driveForward();
    void driveLeft();
    void driveRight();
    void stopDrive();
    void driveFinished();

    void sendStatusMessage();
    bool isGpsSignalValid();
    bool _finished;
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);
#endif // MIBAUTOPILOT_H
