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

    void setTarget(QPointF p, int id);
    void updateDriveState();
    void sendStatusMessage();

    void calcNewAngleAndDist();

    AutopilotSettings * _autopilotSettings;
    DriveConfigSettings * _driveSettings;

    GPIO *gpio();
    AbstractDriveModel * _drive_model;
    DrivingState * _drive_state;

    bool _ap_enabled;
    bool _invalid_gps_data;
    bool _new_gps_data;

    QPointF _ap_target;
    int _ap_target_id;
    QString _ap_drive_type;

    double _ap_relative_angle;
    double _ap_distance;

    bool _ap_finished;
    bool _wait_for_second_gps;
    GPSData _current_gps_data;
    QPointF _last_gps_pos;
    QPointF _current_gps_pos;

    QElapsedTimer _turn_timer;
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);
#endif // MIBAUTOPILOT_H
