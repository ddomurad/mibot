#include <mibLogger.h>

#include "inc/mibDriveStrategy.h"
#include <mibGlobalAccess.h>

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
    _config(nullptr),
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

    if(_config != nullptr)
        delete _config;
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

    ResourceWrapper<DriveConfigRes> *repo = new ResourceWrapper<DriveConfigRes> (GlobalAccess::get().Repository());
    _config = repo->getByID( _connection->SocketObj()->StrategyConfig() );
    delete repo;

    if(_config == nullptr)
    {
        LOG_ERROR( QString("Can;t load strategy configuration (id: %1)").arg(_connection->SocketObj()->StrategyConfig()));
        return false;
    }else
    {
        LOG_IMPORTANT( QString("Driver strategy configuration laoded: '%1'").arg(_config->Alias()) );
    }

    if(!gpio()->Init())
    {
        LOG_ERROR("Can't initialize GPIO.");
        return false;
    }

    gpio()->DisableAllPwms();

    gpio()->SetPinMode( _config->LeftAPin(), PinMode::Output );
    gpio()->SetPinMode( _config->LeftBPin(), PinMode::Output );
    gpio()->SetPinMode( _config->RightAPin(), PinMode::Output );
    gpio()->SetPinMode( _config->RightBPin(), PinMode::Output );

    if(!gpio()->EnablePwm( _config->LeftPwmPin(), true) ||
    !gpio()->EnablePwm( _config->RightPwmPin(), true))
    {
            LOG_ERROR("Initializing PWM error.");
    }

    gpio()->SetPwmValue( _config->LeftPwmPin(), 0 );
    gpio()->SetPwmValue( _config->RightPwmPin(), 0 );

    _state = new DrivingState();
    _model = new VehicleDriveModel();

    _model->Init(
                new WheelDriver( _config->LeftAPin(), _config->LeftBPin(), _config->LeftPwmPin(), gpio() ),
                new WheelDriver( _config->RightAPin(), _config->RightBPin(), _config->RightPwmPin(), gpio() ),
                _state );

    _state->fake_gpio = _config->UseFakeGPIO() ? 0x01 : 0x00;

    _protocol_handler.InitRegisters( 0x04, 0x02);
    _protocol_handler.SetReadableAddr(ADDRR_IS_SIMULATION_MODE,   & (_state->fake_gpio) );
    _protocol_handler.SetReadableAddr(ADDRR_BRAKE,                & (_state->brake) );

    _protocol_handler.SetWritableAddr(ADDRW_BRAKE,  &(_state->brake) );
    _protocol_handler.SetWritableAddr(ADDRW_TURBO,  &(_state->turbo) );
    _protocol_handler.SetWritableAddr(ADDRW_DRIVE,  &(_state->drive_axis) );
    _protocol_handler.SetWritableAddr(ADDRW_TURN,   &(_state->turn_axis) );


     _emergency_brake_timer.start();
    _update_timer->start( _config->UpdateRatio());

    return true;
}

void DriveStartegy::onUpdate()
{
    if(_emergency_brake_timer.elapsed() >= _config->EmergencyBreakTimeout())
    {
        if(_state->brake != 0x01)
        {
            qDebug() << "emergency brakte: " << _emergency_brake_timer.elapsed();
        }
            _state->brake = 0x01;
    }

    _model->Update( float(_config->UpdateRatio())/1000.0f );
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
    return GPIO::GetGPIO( !_config->UseFakeGPIO() );
}

int DriveStartegy::_cnt = 0;

AbstractSocketStrategy *createStrategy(Connection *connection)
{
    return new DriveStartegy(connection);
}
