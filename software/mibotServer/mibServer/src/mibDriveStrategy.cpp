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
    AbstractSocketStrategy(connection)
{
    _update_timer = new QTimer( this );
    connect(_update_timer, SIGNAL(timeout()), this, SLOT(onUpdate()));
}

DriveStartegy::~DriveStartegy()
{
    _update_timer->stop();
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

    gpio()->SetPinMode( _config.gpio_left_a, 1 );
    gpio()->SetPinMode( _config.gpio_left_b, 1 );
    gpio()->SetPinMode( _config.gpio_right_a, 1 );
    gpio()->SetPinMode( _config.gpio_right_b, 1 );

    gpio()->EnablePwm( _config.gpio_left_pwm, true);
    gpio()->EnablePwm( _config.gpio_right_pwm, true);

    gpio()->SetPwmValue( _config.gpio_left_pwm, 0 );
    gpio()->SetPwmValue( _config.gpio_right_pwm, 0 );

    _protocol_handler.InitRegisters( 0x04, 0x02);
    _protocol_handler.SetReadableAddr(ADDRR_IS_SIMULATION_MODE,   & _config.fake_gpio);
    _protocol_handler.SetReadableAddr(ADDRR_EMERGENCY_BRAKE, & _state.emergency_brake);

    _protocol_handler.SetWritableAddr(ADDRW_DRIVE_MODEL,          &_state.drive_model);
    _protocol_handler.SetWritableAddr(ADDRW_BRAKE,                &_state.brake);
    _protocol_handler.SetWritableAddr(ADDRW_LEFT_MOTORS_SPEED,    &_state.left_motors_speed);
    _protocol_handler.SetWritableAddr(ADDRW_RIGHT_MOTORS_SPEED,   &_state.right_motors_speed);

     _emergency_brake_timer.start();
    _update_timer->start( _config.gpio_update_ratio );

    return true;
}

void DriveStartegy::onUpdate()
{
    if(_emergency_brake_timer.elapsed() >= _config.emergency_break_timeout)
    {
        if(_state.emergency_brake != 0x01)
        {
            qDebug() << "emergency brakte: " << _emergency_brake_timer.elapsed();
        }

            _state.emergency_brake = 0x01;
    }
    else
        _state.emergency_brake = 0x00;

    qint8 left_pwm = qAbs<qint8>(_state.left_motors_speed);
    qint8 rigth_pwm = qAbs<qint8>(_state.right_motors_speed);

    left_pwm = _clamp_value<qint8>( left_pwm, 0, 100);
    rigth_pwm = _clamp_value<qint8>( rigth_pwm, 0, 100);

    bool left_dir = _state.left_motors_speed >= 0;
    bool right_dir = _state.right_motors_speed >= 0;

    if(_state.brake || _state.emergency_brake)
    {
        gpio()->SetPin( _config.gpio_left_a, 0 );
        gpio()->SetPin( _config.gpio_left_b, 0 );
        gpio()->SetPin( _config.gpio_right_a, 0 );
        gpio()->SetPin( _config.gpio_right_b, 0 );
        gpio()->SetPwmValue( _config.gpio_left_pwm, 0 );
        gpio()->SetPwmValue( _config.gpio_right_pwm, 0 );
    }else
    {
        gpio()->SetPin( _config.gpio_left_a, left_dir );
        gpio()->SetPin( _config.gpio_left_b, !left_dir );
        gpio()->SetPin( _config.gpio_right_a, right_dir );
        gpio()->SetPin( _config.gpio_right_b, !right_dir );
        gpio()->SetPwmValue( _config.gpio_left_pwm, left_pwm );
        gpio()->SetPwmValue( _config.gpio_right_pwm, rigth_pwm );
    }
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


DriverStartegyState::DriverStartegyState()
{
    brake               = -1;
    emergency_brake     = -1;
    drive_model         = -1;

    left_motors_speed     = -1;
    right_motors_speed    = -1;
}
