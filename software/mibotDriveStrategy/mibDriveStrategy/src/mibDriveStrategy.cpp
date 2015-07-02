#include <mibLogger.h>

#include "inc/mibDriveStrategy.h"
#include <mibSettingsClient.h>

template <typename T>
inline T _clamp_value(T v, T min, T max)
{
    if( v < min) return min;
    if(v > max) return max;
    return v;
}

using namespace mibot;

DriveStartegy::DriveStartegy(Connection *connection) :
    AbstractSocketStrategy(connection),
    _driveSettings(nullptr),
    _model(nullptr)
{
    _update_timer = new QTimer( this );
    connect(_update_timer, SIGNAL(timeout()), this, SLOT(onUpdate()));
}

DriveStartegy::~DriveStartegy()
{
    _update_timer->stop();
    if(_model != nullptr)
        delete _model;

    gpio()->DisableAllPwms();
    _cnt--;

    if(_driveSettings != nullptr)
        _driveSettings->Release();
}

void DriveStartegy::processNewData(QByteArray data)
{
    _emergency_brake_timer.restart();
    for(int i=0; i< data.count(); i++)
    {
        _protocol_handler.PushByte( data.at(i) );
        if(_protocol_handler.BytesToRead() != 0)
        {
            uchar send_data_size = _protocol_handler.BytesToRead();
            qint8 * send_data = _protocol_handler.FlushOutputBuffer();
            _connection->TcpSocket->write( (const char*)send_data, int(send_data_size));
        }
    }
}

bool DriveStartegy::init()
{
    if(_cnt != 0)
    {
        LOG_ERROR("Can't initailize drive stategy. One drive strategy already exists.");
        return false;
    }

    _cnt++;

    _driveSettings = SettingsClient::CreateReource<DriveConfigSettings>(_connection->SocketObj->strategyConfig->value);

    if(!_driveSettings->Sync())
    {
        LOG_ERROR(QString("Can't get driver strategy '%1'").arg(_driveSettings->Resource()));
        _driveSettings->Release();
        _driveSettings = nullptr;
        return false;
    }

    LOG_IMPORTANT( QString("Driver strategy settings loadet: '%1'").arg(_driveSettings->alias->value));

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

    _protocol_handler.InitRegisters( 0x04, 0x02);
    _protocol_handler.SetReadableAddr(ADDRR_IS_SIMULATION_MODE,   & (_state->fake_gpio) );
    _protocol_handler.SetReadableAddr(ADDRR_BRAKE,                & (_state->brake) );

    _protocol_handler.SetWritableAddr(ADDRW_BRAKE,  &(_state->brake) );
    _protocol_handler.SetWritableAddr(ADDRW_TURBO,  &(_state->turbo) );
    _protocol_handler.SetWritableAddr(ADDRW_DRIVE,  &(_state->drive_axis) );
    _protocol_handler.SetWritableAddr(ADDRW_TURN,   &(_state->turn_axis) );


     _emergency_brake_timer.start();
    _update_timer->start( _driveSettings->updateRatio->value);

    return true;
}

void DriveStartegy::onUpdate()
{
    if(_emergency_brake_timer.elapsed() >= _driveSettings->emergencyBreakTimeout->value)
    {
        if(_state->brake != 0x01)
        {
            qDebug() << "emergency brakte: " << _emergency_brake_timer.elapsed();
        }
            _state->brake = 0x01;
    }

    _model->Update( float(_driveSettings->updateRatio->value)/1000.0f );
}

//template <typename T>
//bool DriveStartegy::getValue(T *val, GlobalConfigRes *res)
//{
//    bool ok;
//    *val = (T)res->Value().toInt(&ok);
//    if(!ok)
//    {
//        LOG_ERROR( QString("Can't translate config '%1' to int.").arg( res->Key()));
//        return false;
//    }

//    return true;
//}

GPIO *DriveStartegy::gpio()
{
    return GPIO::GetGPIO( !_driveSettings->useFakeGPIO->value);
}

int DriveStartegy::_cnt = 0;

AbstractSocketStrategy *createStrategy(Connection *connection)
{
    return new DriveStartegy(connection);
}
