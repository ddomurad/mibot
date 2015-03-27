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

    _config = DriveStartegyConfig();

    ResourcesSet<GlobalConfigRes> * config_set =
            GlobalAccess::AllGlobalConfigsForSubsystem("drive");

    if(config_set == nullptr)
    {
        LOG_ERROR("Can't read global configs.");
        return false;
    }

    bool ok = true;
    for( int i=0; i< config_set->Count(); i++)
    {
        GlobalConfigRes * cfg = config_set->At(i);
        if(cfg->Key() == "gpio_left_a")
            ok &= getValue(&_config.gpio_left_a, cfg);
        else if(cfg->Key() == "gpio_left_b")
            ok &= getValue(&_config.gpio_left_b, cfg);
        else if(cfg->Key() == "gpio_left_pwm")
            ok &= getValue(&_config.gpio_left_pwm, cfg);
        else if(cfg->Key() == "gpio_right_a")
            ok &= getValue(&_config.gpio_right_a, cfg);
        else if(cfg->Key() == "gpio_right_b")
            ok &= getValue(&_config.gpio_right_b, cfg);
        else if(cfg->Key() == "gpio_right_pwm")
            ok &= getValue(&_config.gpio_right_pwm, cfg);
        else if(cfg->Key() == "emergency_break_timeout")
            ok &= getValue(&_config.emergency_break_timeout, cfg);
        else if(cfg->Key() == "gpio_update_ratio")
            ok &= getValue(&_config.gpio_update_ratio, cfg);
        else if(cfg->Key() == "fake_gpio")
            ok &= getValue(&_config.fake_gpio, cfg);
    }

    delete config_set;

    if(!checkConfigs())
    {
        LOG_ERROR("Not all config are defined.");
        return false;
    }

    if(!gpio()->Init())
    {
        LOG_ERROR("Can't initialize GPIO.");
        return false;
    }

    gpio()->DisableAllPwms();

    gpio()->SetPinMode( _config.gpio_left_a, PinMode::Output );
    gpio()->SetPinMode( _config.gpio_left_b, PinMode::Output );
    gpio()->SetPinMode( _config.gpio_right_a, PinMode::Output );
    gpio()->SetPinMode( _config.gpio_right_b, PinMode::Output );

    if(!gpio()->EnablePwm( _config.gpio_left_pwm, true) ||
    !gpio()->EnablePwm( _config.gpio_right_pwm, true))
    {
            LOG_ERROR("Initializing PWM error.");
    }

    gpio()->SetPwmValue( _config.gpio_left_pwm, 0 );
    gpio()->SetPwmValue( _config.gpio_right_pwm, 0 );

    _state = new DrivingState();
    _model = new VehicleDriveModel();

    _model->Init(
                new WheelDriver( _config.gpio_left_a, _config.gpio_left_b, _config.gpio_left_pwm, gpio() ),
                new WheelDriver( _config.gpio_right_a, _config.gpio_right_b, _config.gpio_right_pwm, gpio() ),
                _state );

    _protocol_handler.InitRegisters( 0x04, 0x02);
    _protocol_handler.SetReadableAddr(ADDRR_IS_SIMULATION_MODE,   & (_config.fake_gpio) );
    _protocol_handler.SetReadableAddr(ADDRR_BRAKE,                & (_state->brake) );

    _protocol_handler.SetWritableAddr(ADDRW_BRAKE,  &(_state->brake) );
    _protocol_handler.SetWritableAddr(ADDRW_TURBO,  &(_state->turbo) );
    _protocol_handler.SetWritableAddr(ADDRW_DRIVE,  &(_state->drive_axis) );
    _protocol_handler.SetWritableAddr(ADDRW_TURN,   &(_state->turn_axis) );


     _emergency_brake_timer.start();
    _update_timer->start( _config.gpio_update_ratio );

    return true;
}

void DriveStartegy::onUpdate()
{
    if(_emergency_brake_timer.elapsed() >= _config.emergency_break_timeout)
    {
        if(_state->brake != 0x01)
        {
            qDebug() << "emergency brakte: " << _emergency_brake_timer.elapsed();
        }
            _state->brake = 0x01;
    }

    _model->Update( float(_config.gpio_update_ratio)/1000.0f );
}

bool DriveStartegy::getValue(qint8 *val, GlobalConfigRes *res)
{
    bool ok;
    *val = (char)res->Value().toInt(&ok);
    if(!ok)
    {
        LOG_ERROR( QString("Can't translate config '%1' to int.").arg( res->Key()));
        return false;
    }

    return true;
}

bool DriveStartegy::checkConfigs()
{
    if(_config.emergency_break_timeout == -1
            || _config.gpio_left_a == -1
            || _config.gpio_left_b == -1
            || _config.gpio_left_pwm == -1
            || _config.gpio_right_a == -1
            || _config.gpio_right_b == -1
            || _config.gpio_right_pwm == -1
            || _config.gpio_update_ratio == -1
            || _config.fake_gpio == -1)
        return false;

    return true;
}

GPIO *DriveStartegy::gpio()
{
    if(_config.fake_gpio == 0)
        return GPIO::GetGPIO(true);
    else return GPIO::GetGPIO(false);
}

int DriveStartegy::_cnt = 0;

DriveStartegyConfig::DriveStartegyConfig()
{
    gpio_left_a     = -1;
    gpio_left_b     = -1;
    gpio_left_pwm   = -1;

    gpio_right_a    = -1;
    gpio_right_b    = -1;
    gpio_right_pwm  = -1;

    // system
    gpio_update_ratio       = -1;
    emergency_break_timeout = -1;
    fake_gpio               = -1;
}
