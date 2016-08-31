#include "mibLogger.h"
#include "inc/mibAutopilot.h"

using namespace mibot;
#define THE_PI 3.1415926535

Autopilot::Autopilot(Connection *connection) :
    AbstractSocketStrategy(connection)
{
    _watch_dog_timer = new QTimer(this);
    _driver_update_timer = new QTimer(this);
    _state_update_timer = new QTimer(this);

    connect(_watch_dog_timer, SIGNAL(timeout()), this, SLOT(onWatchDogTimer()));
    connect(_driver_update_timer, SIGNAL(timeout()), this, SLOT(onDriverTimer()));
    connect(_state_update_timer, SIGNAL(timeout()), this, SLOT(onStateTimer()));

    _watchdog_state = false;
    _ap_enabled = false;
    _ap_finished = false;
    _ap_relative_angle = 0.0;
    _ap_distance = 0.0;
    _wait_for_second_gps = true;
    _ap_drive_type = "";
}

Autopilot::~Autopilot()
{
    _watch_dog_timer->stop();
    _driver_update_timer->stop();

    if(_drive_model != nullptr)
        delete _drive_model;

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
            return false;
    }

    gpio()->SetPwmValue( _driveSettings->leftPwmPin->value, 0 );
    gpio()->SetPwmValue( _driveSettings->rightPwmPin->value, 0 );

    _drive_state = new DrivingState();
    _drive_model = new VehicleDriveModel();

    _drive_model->Init(
                new WheelDriver( _driveSettings->leftAPin->value, _driveSettings->leftBPin->value, _driveSettings->leftPwmPin->value, gpio() ),
                new WheelDriver( _driveSettings->rightAPin->value, _driveSettings->rightBPin->value, _driveSettings->rightPwmPin->value, gpio() ),
                _drive_state );

    _drive_state->fake_gpio = _driveSettings->useFakeGPIO->value ? 0x01 : 0x00;

    _watch_dog_timer->setInterval(_autopilotSettings->watchDogTimerInterval->value);
    _watch_dog_timer->start();

    _driver_update_timer->setInterval(_driveSettings->updateRatio->value);
    _driver_update_timer->start();

    _state_update_timer->setInterval(_autopilotSettings->statuTimerInterval->value);
    _state_update_timer->start();

    _last_gps_pos = QPointF(0,0);
    _current_gps_pos = QPointF(0,0);

    _wait_for_second_gps = true;
    _ap_enabled = false;
    _invalid_gps_data = true;

    return true;
}

void Autopilot::onWatchDogTimer()
{
    if(_watchdog_state)
    {
        if(_drive_state->brake != 0x01)
            LOG_WARNING("WDT Emergency brake");

        _drive_state->brake = 0x01;
    }

    _watchdog_state = true;
}

void Autopilot::onDriverTimer()
{
    updateDriveState();
    _drive_model->Update( float(_driveSettings->updateRatio->value)/1000.0f );
}

void Autopilot::onStateTimer()
{
    sendStatusMessage();
}

void Autopilot::onGpsData(GPSData data)
{
    if(!data.isValid)
    {
        if(_ap_enabled)
        {
            LOG_WARNING("Invalid GPS DATA. Disabling autopilot.");
            _ap_enabled = false;
            _invalid_gps_data = true;
        }
        return;
    }

    if(_current_gps_data.position.latitude != data.position.latitude
        || _current_gps_data.position.longitude != data.position.longitude)
    {
        _invalid_gps_data = false;
        _last_gps_pos = _current_gps_pos;
        _current_gps_data = data;
        _current_gps_pos.setX(data.position.longitude);
        _current_gps_pos.setY(data.position.latitude);

        if(_last_gps_pos.x() != 0 && _last_gps_pos.y() != 0 && _wait_for_second_gps)
        {
            LOG_INFO("Second gps pos received.");
            _wait_for_second_gps = false;
        }else
        {
            LOG_INFO("Waiting for second gps pos.");
        }

        LOG_DEBUG(QString("Gps position received: [%1, %2]")
                  .arg(_current_gps_pos.x())
                  .arg(_current_gps_pos.y()));

        _new_gps_data = true;
    }
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
            _drive_state->brake = 0x00;

        _ap_enabled = enabled;
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

void Autopilot::setTarget(QPointF p, int id)
{
    if(_ap_target != p)
    {
        LOG_INFO(QString("New target: [%1, %2] id %3")
             .arg(p.x())
             .arg(p.y())
             .arg(id));
    }

    _ap_target = p;
    _ap_target_id = id;
    _ap_finished = false;
}

void Autopilot::updateDriveState()
{
    if(!_ap_enabled || _ap_finished || _invalid_gps_data)
    {
        _drive_state->turbo = 0x0;
        _drive_state->turn_axis = 0x0;
        _drive_state->drive_axis = 0x0;
        _ap_drive_type = "";
        return;
    }else
    {
        _drive_state->turbo = 0x01;
        _drive_state->drive_axis = 0x01;
    }

    if(_new_gps_data)
    {
        LOG_DEBUG("New GPS data");

        _new_gps_data = false;
        calcNewAngleAndDist();

        if(_ap_distance <= _autopilotSettings->maxDistance->value)
        {
            _ap_finished = true;
            return;
        }

        if(fabs(_ap_relative_angle) > _autopilotSettings->angleTreshold->value)
        {
            if(_ap_relative_angle > 0)
            {
                LOG_DEBUG("Start turning (a>0)");
                _drive_state->turn_axis = 1;
                _ap_drive_type = "L";
            }
            else
            {
                LOG_DEBUG("Start turning (a<0)");
                _drive_state->turn_axis = -1;
                _ap_drive_type = "R";
            }

            _turn_timer.restart();
        }else
        {
            _drive_state->turn_axis = 0;
            _ap_drive_type = "F";
        }

        return;
    }

    if(_drive_state->turn_axis != 0)
    {
        if(_turn_timer.elapsed() > _autopilotSettings->turnTime->value)
        {
            LOG_DEBUG("Stop turning");
            _drive_state->turn_axis = 0;
            _ap_drive_type = "F";
        }
    }
}

GPIO *Autopilot::gpio()
{
    return GPIO::GetGPIO( !_driveSettings->useFakeGPIO->value);
}

void Autopilot::sendStatusMessage()
{
    QString msg;
    if(!_invalid_gps_data)
    {
        if(_ap_finished)
        {
            msg = QString("{\"distance\":%1,\"angle\":%2,\"arrived_at\":%3}")
                    .arg(_ap_distance) // distance
                    .arg(_ap_relative_angle) // angle
                    .arg(_ap_target_id);
        }else
        {
            msg = QString("{\"distance\":%1,\"angle\":%2,\"drive_type\":\"%3\"}")
                    .arg(_ap_distance) //disance
                    .arg(_ap_relative_angle) //angle
                    .arg(_ap_drive_type);
        }
    }else
    {
        msg = QString("{\"no_gps\":1}");
    }

    _connection->TcpSocket->write(msg.toUtf8());
}

void Autopilot::calcNewAngleAndDist()
{
    //dist
    QPointF _relative_target = _ap_target - _current_gps_pos;
    _ap_distance = sqrt(pow(_relative_target.x(),2) + pow(_relative_target.y(),2));

    //relative angle

    //a1 - angle to target
    double a1 = atan2(_relative_target.y(), _relative_target.x());

    if(!_wait_for_second_gps)
    {
        //a2 - angle last position - cource
        QPointF relative_disp = _last_gps_pos - _current_gps_pos;
        double a2 = atan2(relative_disp.y(), relative_disp.x());

        _ap_relative_angle = a2 - a1;
        if(_ap_relative_angle > THE_PI) _ap_relative_angle-=2*THE_PI;
        if(_ap_relative_angle < -THE_PI) _ap_relative_angle+=2*THE_PI;
    }else
    {
        _ap_relative_angle = a1;
    }

    LOG_DEBUG("New distance calculated: " + QString::number(_ap_distance*180.0/THE_PI));
    LOG_DEBUG("New relative angle calculated: " + QString::number(_ap_relative_angle*180.0/THE_PI));
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

//qreal Autopilot::getRelativeAngleTo(qreal ar, QPointF gp)
//{
//    //    qreal ar = gpsCource() - 180.0;
//    //    QPointF gp = gpsPosition();

//    ar -= 180.0;
//    QPointF rp = _target_location - gp;
//    qreal ap = atan2(rp.y(), rp.x()) * 180.0/3.1415926535;

//    qreal a = ap - ar;

//    if(a > 180) a-=360;
//    if(a < -180) a+=360;
//    return a;
//}


//qreal Autopilot::calcTurnData()
//{
////    if(_gps_data.length() < 3)
////    {
////        LOG_ERROR("_gps_data.length() < 3");
////        return 0.0;
////    }

////    QPointF last_gps_pos = QPointF(_gpsSensor->Readings().position.longitude,
////                                   _gpsSensor->Readings().position.latitude);

////    qreal av_angle = 0;
////    for(int i=1;i<_gps_data.length();i++)
////        av_angle += _gps_data[i].movement.course;

////    av_angle /= qreal(_gps_data.length() - 1);

////    return getRelativeAngleTo(av_angle, last_gps_pos);
//}



mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new Autopilot(connection);
}
