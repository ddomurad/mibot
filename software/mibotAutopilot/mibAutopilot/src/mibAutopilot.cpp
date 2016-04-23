#include "mibLogger.h"
#include "inc/mibAutopilot.h"

using namespace mibot;

Autopilot::Autopilot(Connection *connection) :
    AbstractSocketStrategy(connection)
{
    _watch_dog_timer = new QTimer(this);
    _driver_update_timer = new QTimer(this);
    _state_timer = new QTimer(this);

    connect(_watch_dog_timer, SIGNAL(timeout()), this, SLOT(onWatchDogTimer()));
    connect(_driver_update_timer, SIGNAL(timeout()), this, SLOT(onDriverTimer()));
    connect(_state_timer, SIGNAL(timeout()), this, SLOT(onStateTimer()));

    _watchdog_state = false;
    _autopilot_enabled = false;
    _finished = false;
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

    _gpsSensor = GPSSensor::get();

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

    _state_timer->setInterval(_autopilotSettings->statuTimerInterval->value);
    _state_timer->start();

    return true;
}

void Autopilot::onWatchDogTimer()
{
    if(_watchdog_state)
    {
        if(_state->brake != 0x01)
        {
            LOG_WARNING("emergency brake");
        }

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
        if(enabled != _autopilot_enabled)
        {
            if(enabled) enableAutopilot();
            else disableAutopilot();
        }
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

qreal Autopilot::getDistace()
{
    QPointF r = _target_location - gpsPosition();
    return sqrt(pow(r.x(),2) + pow(r.y(),2));
}

qreal Autopilot::getRelativeAngle()
{
    QPointF gp = gpsPosition();
    qreal ar = gpsCource() - 180.0;
    QPointF rp = _target_location - gp;

    qreal ap = atan2(rp.y(), rp.x()) * 180.0/3.1415926535;

    qreal a = ap - ar;
    if(a > 180) a-=360;
    if(a < -180) a+=360;
    return a;
}

QPointF Autopilot::gpsPosition()
{
    return QPointF(_gpsSensor->Readings().position.longitude,
                   _gpsSensor->Readings().position.latitude);
}

qreal Autopilot::gpsCource()
{
    return _gpsSensor->Readings().movement.course;
}

void Autopilot::updateAutopilot()
{
    if(!_autopilot_enabled)
    {
        stopDrive();
        return;
    }

    if(!isGpsSignalValid())
        return;

    qreal dist = getDistace();
    if(dist <= _autopilotSettings->maxDistance->value)
    {
        driveFinished();
        return;
    }

    qreal ra = getRelativeAngle();

    if(abs(ra) <= 10)
        driveForward();
    else if(ra > 0)
        driveRight();
    else
        driveLeft();
}

void Autopilot::setTarget(QPointF p, int id)
{
    LOG_INFO(QString("New target set: (%1,%2), id: %3")
             .arg(p.x())
             .arg(p.y())
             .arg(id));

    _finished = false;
    _target_location = p;
    _target_id = id;
}

void Autopilot::enableAutopilot()
{
    LOG_INFO("Autopilot enabled");
    _autopilot_enabled = true;
    _state->brake = 0x00;
}

void Autopilot::disableAutopilot()
{
    LOG_INFO("Autopilot disabled");
    _autopilot_enabled = false;
    stopDrive();
}

void Autopilot::driveForward()
{
    _state->drive_axis = 1;
    _state->turn_axis = 0;
    _state->turbo = 1;
}

void Autopilot::driveLeft()
{
    _state->drive_axis = 1;
    _state->turn_axis = -1;
    _state->turbo = 1;
}

void Autopilot::driveRight()
{
    _state->drive_axis = 1;
    _state->turn_axis = 1;
    _state->turbo = 1;
}

void Autopilot::stopDrive()
{
    _state->drive_axis = 0;
    _state->turn_axis = 0;
    _state->turbo = 0;
}

void Autopilot::driveFinished()
{
    disableAutopilot();
    _finished = true;
}

void Autopilot::sendStatusMessage()
{
    QString msg;
    if(_gpsSensor->Readings().isValid)
    {
        if(_finished)
        {
            msg = QString("{\"distance\":%1,\"angle\":%2,\"arrived_at\":%3}")
                    .arg(getDistace())
                    .arg(getRelativeAngle())
                    .arg(_target_id);
        }else
        {
            msg = QString("{\"distance\":%1,\"angle\":%2}")
                    .arg(getDistace())
                    .arg(getRelativeAngle());
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


mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new Autopilot(connection);
}
