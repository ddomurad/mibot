#include "mibLogger.h"
#include "inc/mibAutopilot.h"

using namespace mibot;

Autopilot::Autopilot(Connection *connection) :
    AbstractSocketStrategy(connection)
{
    _watch_dog_timer = new QTimer(this);
    _driver_update_timer = new QTimer(this);
    _state_update_timer = new QTimer(this);

    connect(_watch_dog_timer, SIGNAL(timeout()), this, SLOT(onWatchDogTimer()));
    connect(_driver_update_timer, SIGNAL(timeout()), this, SLOT(onDriverTimer()));
    connect(_state_update_timer, SIGNAL(timeout()), this, SLOT(onStateTimer()));

    _angleToTurn = 0;
    _watchdog_state = false;
    _finished = false;
    _gps_data_recived = false;
    _ap_state = Autopilot::DISABLED;
}

Autopilot::~Autopilot()
{
    _watch_dog_timer->stop();
    _driver_update_timer->stop();

    if(_model != nullptr)
        delete _model;

    gpio()->DisableAllPwms();

    if(_autopilotSettings != nullptr)
        _autopilotSettings->Release();

    if(_driveSettings != nullptr)
        _driveSettings->Release();
}

void Autopilot::processNewData(QByteArray data)
{
    protocol.PushData(data);
    tryRemoveCoruptedJSONProtocolData();
    while(protocol.ContainsValidJSONObject())
    {
        QJsonObject obj = protocol.GetPendingObject();
        processCommand(obj);
        tryRemoveCoruptedJSONProtocolData();
    }
}

bool Autopilot::init()
{
    LOG_IMPORTANT("Initializing Autopilot");

    _autopilotSettings = SettingsClient::CreateReource<AutopilotSettings>(_connection->SocketObj->strategyConfig->value);

    if(!_autopilotSettings->Sync())
    {
        LOG_ERROR(QString("Can't get autopilot settings '%1'").arg( _connection->SocketObj->strategyConfig->value ));
        return false;
    }

    _driveSettings = SettingsClient::CreateReource<DriveConfigSettings>(_autopilotSettings->driveSettings->value);
    if(!_driveSettings->Sync())
    {
        LOG_ERROR(QString("Can't get drive settings '%1'").arg( _autopilotSettings->driveSettings->value ));
        return false;
    }

    if(!GPSSensor::get()->Start())
    {
        LOG_ERROR("Can't initialzie GPSSensor");
        return false;
    }

    _time_factor = _autopilotSettings->baseTimeFactor->value;

    _gpsSensor = GPSSensor::get();
    connect(_gpsSensor, SIGNAL(onNewGpsData(GPSData)), this, SLOT(onGpsData(GPSData)));

    if(!gpio()->Init())
    {
        LOG_ERROR("Can't initialize GPIO.");
        return false;
    }

    gpio()->DisableAllPwms();

    gpio()->SetPinMode( _driveSettings->leftAPin->value, PinMode::Output );
    gpio()->SetPinMode( _driveSettings->leftBPin->value, PinMode::Output );
    gpio()->SetPinMode( _driveSettings->rightAPin->value, PinMode::Output );
    gpio()->SetPinMode( _driveSettings->rightBPin->value, PinMode::Output );

    if(!gpio()->EnablePwm( _driveSettings->leftPwmPin->value, true) ||
        !gpio()->EnablePwm( _driveSettings->rightPwmPin->value, true))
    {
            LOG_ERROR("Initializing PWM error.");
    }

    gpio()->SetPwmValue( _driveSettings->leftPwmPin->value, 0 );
    gpio()->SetPwmValue( _driveSettings->rightPwmPin->value, 0 );

    _state = new DrivingState();
    _model = new VehicleDriveModel();

    _model->Init(
                new WheelDriver( _driveSettings->leftAPin->value, _driveSettings->leftBPin->value, _driveSettings->leftPwmPin->value, gpio() ),
                new WheelDriver( _driveSettings->rightAPin->value, _driveSettings->rightBPin->value, _driveSettings->rightPwmPin->value, gpio() ),
                _state );

    _state->fake_gpio = _driveSettings->useFakeGPIO->value ? 0x01 : 0x00;

    _watch_dog_timer->setInterval(_autopilotSettings->watchDogTimerInterval->value);
    _watch_dog_timer->start();

    _driver_update_timer->setInterval(_driveSettings->updateRatio->value);
    _driver_update_timer->start();

    _state_update_timer->setInterval(_autopilotSettings->statuTimerInterval->value);
    _state_update_timer->start();

    return true;
}

void Autopilot::onWatchDogTimer()
{
    if(_watchdog_state)
    {
        if(_state->brake != 0x01)
            LOG_WARNING("Emergency brake");

        disableAutopilot();
        _state->brake = 0x01;
    }

    _watchdog_state = true;
}

void Autopilot::onDriverTimer()
{
    updateAutopilot();
    _model->Update( float(_driveSettings->updateRatio->value)/1000.0f );
}

void Autopilot::onStateTimer()
{
    sendStatusMessage();
}

void Autopilot::onGpsData(GPSData data)
{
    if(_ap_state == Autopilot::DRIVE_FORWARD)
        _gps_data.append(data);

    _gps_data_recived = true;
}

void Autopilot::tryRemoveCoruptedJSONProtocolData()
{
    if(protocol.IsDataCorupted())
    {
        protocol.RemoveCoruptedData();
        if(protocol.IsDataCorupted())
        {
            LOG_WARNING("Can't remove json procol corupted data. All data is removed.");
            protocol.RemoveAllData();
        }
        else
        {
            LOG_WARNING("Corupted protocol data removed.");
        }
    }
}

void Autopilot::processCommand(QJsonObject &obj)
{
    if(obj["autopilot_enabled"].isBool())
    {
        _watchdog_state = false;
        bool enabled = obj["autopilot_enabled"].toBool();

        if(enabled)
            _state->brake = 0x00;

        if(enabled && _ap_state == Autopilot::DISABLED)
            enableAutopilot();
        else if(!enabled && _ap_state != Autopilot::DISABLED)
            disableAutopilot();

    }

    if(obj["gps_pos"].isArray() && obj["point_id"].isDouble())
    {
        QJsonArray arr = obj["gps_pos"].toArray();
        double log = arr.at(0).toDouble();
        double lat = arr.at(1).toDouble();
        int id = obj["point_id"].toDouble();

        setTarget(QPointF(log,lat), id);
    }
}

/*
 *  // incoming
 {
   "point_id":123,
   "gps_point":[123.12, 123.12],
   "autopilot_enabled":true
 }
 // for update
 {
    "autopilot_enabled":true
 }

 //out
 {
    "distance":123,
    "angle",
    "arrived_at":[point_id]
 }

*/

GPIO *Autopilot::gpio()
{
    return GPIO::GetGPIO( !_driveSettings->useFakeGPIO->value);
}

qreal Autopilot::getRelativeAngleTo(qreal ar, QPointF gp)
{
    //    qreal ar = gpsCource() - 180.0;
    //    QPointF gp = gpsPosition();

    ar -= 180.0;
    QPointF rp = _target_location - gp;
    qreal ap = atan2(rp.y(), rp.x()) * 180.0/3.1415926535;

    qreal a = ap - ar;

    if(a > 180) a-=360;
    if(a < -180) a+=360;
    return a;
}


qreal Autopilot::getDistace()
{
    QPointF gps_pos = QPointF(_gpsSensor->Readings().position.longitude,
            _gpsSensor->Readings().position.latitude);
    QPointF r = _target_location - gps_pos;
    return sqrt(pow(r.x(),2) + pow(r.y(),2));
}


void Autopilot::updateAutopilot()
{
    if(!_autopilotSettings->dynamicTimeFactor->value)
        _time_factor = _autopilotSettings->baseTimeFactor->value;

    if(_ap_state == Autopilot::DISABLED)
        return;

    if(!isGpsSignalValid())
    {
        LOG_WARNING("Gps data invalid");
        disableAutopilot();
        return;
    }

    updateAutoPilotState();
    updateMotors();
    updateTargetState();
}

void Autopilot::setTarget(QPointF p, int id)
{
    if(_target_location == p && _target_id == id)
        return;

    LOG_INFO(QString("New target set: (%1,%2), id: %3")
             .arg(p.x())
             .arg(p.y())
             .arg(id));

    _finished = false;
    _target_location = p;
    _target_id = id;
}

void Autopilot::sendStatusMessage()
{
    QString msg;
    if(_gpsSensor->Readings().isValid)
    {
        if(_finished)
        {
            msg = QString("{\"distance\":%1,\"angle\":%2,\"arrived_at\":%3}")
                    .arg(getDistace()) // distance
                    .arg(_angleToTurn) // angle
                    .arg(_target_id);
        }else
        {
            msg = QString("{\"distance\":%1,\"angle\":%2}")
                    .arg(getDistace()) //disance
                    .arg(_angleToTurn); //angle
        }
    }else
    {
        msg = QString("{\"no_gps\":1}");
    }

    _connection->TcpSocket->write(msg.toUtf8());
}

bool Autopilot::isGpsSignalValid()
{
    return _gpsSensor->Readings().isValid;
}

void Autopilot::enableAutopilot()
{
    setForwardState();
    _state->brake = 0x00;
    LOG_DEBUG("Auto pilot enabled");
}

void Autopilot::disableAutopilot()
{
    _ap_state = Autopilot::DISABLED;
    setMotors(false, false);
    LOG_DEBUG("Auto pilot disable");
}

void Autopilot::updateMotors()
{
    switch(_ap_state)
    {
        case Autopilot::DISABLED:
        case Autopilot::WAIT_FOR_GPS_DATA:
            setMotors(false, false); break;
        case Autopilot::DRIVE_FORWARD:
            setMotors(true, true); break;
        case Autopilot::DRIVE_LEFT:
            setMotors(true, false); break;
        case Autopilot::DRIVE_RIGHT:
            setMotors(false, true); break;
    }
}

void Autopilot::updateAutoPilotState()
{
    if(_ap_state == Autopilot::DRIVE_FORWARD)
    {
        if(_state_timer.elapsed() >= _autopilotSettings->forwardDriveTime->value)
            setWaitForGpsState();
    }

    if(_ap_state == Autopilot::WAIT_FOR_GPS_DATA)
    {
        if(_gps_data_recived)
            setTurningState();
        return;
    }

    if(_ap_state == Autopilot::DRIVE_LEFT || _ap_state == Autopilot::DRIVE_RIGHT)
    {
        if(_state_timer.elapsed() >= _time_factor * fabs(_angleToTurn))
            setForwardState();
        return;
    }
}

void Autopilot::updateTargetState()
{
    if(getDistace() <= _autopilotSettings->maxDistance->value)
    {
        LOG_INFO( QString("Arrived at target: %d").arg(_target_id));
        _finished = true;
        disableAutopilot();
    }
}

void Autopilot::setWaitForGpsState()
{
    LOG_DEBUG("Set WaitForGpsState");
    _ap_state = Autopilot::WAIT_FOR_GPS_DATA;
    _gps_data_recived = false;
    setMotors(false,false);
}

void Autopilot::setTurningState()
{
    _angleToTurn = calcTurnData();

    if(fabs(_angleToTurn) < _autopilotSettings->angleTreshold->value)
    {
        LOG_DEBUG( QString("Skip turning state: %f").arg(_angleToTurn));
        setForwardState();
        return;
    }else
    {
        if(_angleToTurn > 0)
        {
            _ap_state = Autopilot::DRIVE_LEFT;
            LOG_DEBUG( QString("Set TurnLeftState: %1").arg(_angleToTurn));
        }
        else
        {
            _ap_state = Autopilot::DRIVE_RIGHT;
            LOG_DEBUG( QString("Set TurnRightState: %1").arg(_angleToTurn));
        }
    }

    _state_timer.restart();
}

void Autopilot::setForwardState()
{
    LOG_DEBUG("Set DriveForwardState");
    _ap_state = Autopilot::DRIVE_FORWARD;
    _gps_data.clear();
    _state_timer.restart();
}

qreal Autopilot::calcTurnData()
{
    if(_gps_data.length() < 3)
    {
        LOG_ERROR("_gps_data.length() < 3");
        return 0.0;
    }

    QPointF last_gps_pos = QPointF(_gpsSensor->Readings().position.longitude,
                                   _gpsSensor->Readings().position.latitude);

    qreal av_angle = 0;
    for(int i=1;i<_gps_data.length();i++)
        av_angle += _gps_data[i].movement.course;

    av_angle /= qreal(_gps_data.length() - 1);

    return getRelativeAngleTo(av_angle, last_gps_pos);
}

void Autopilot::setMotors(bool left, bool right)
{
    if(!left && !right)
    {
        _state->turn_axis = 0;
        _state->drive_axis = 0;
        return;
    }

    if(left && !right)
    {
        _state->turn_axis = -1;
        _state->drive_axis = 1;
        _state->turbo = 1;
        return;
    }

    if(!left && right)
    {
        _state->turn_axis = 1;
        _state->drive_axis = 1;
        _state->turbo = 1;
        return;
    }

    if(left && right)
    {
        _state->turn_axis = 0;
        _state->drive_axis = 1;
        _state->turbo = 1;
        return;
    }
}

mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new Autopilot(connection);
}
