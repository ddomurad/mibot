#include "inc/mibControllerStatus.h"

using namespace mibot;

ControllerStatus::ControllerStatus() :
    _write_cnt(0),
    _encoding_state(0),
    _command(0), _addr(0),
    _restart(false)
{
    _out_buffer_data_cnt = 0x00;
    ___drive_model = 0x0C; //break
    ___is_simulation = 0x01; //simulation on
    _loc_driver_model           = 0x0E;
    _loc_left_engine_speed      = 0x00;
    _loc_right_engine_speed     = 0x00;
    _loc_left_engine_curent     = 0x00;
    _loc_right_engine_curent    = 0x00;
    _loc_gear_value             = 0x00;
    _loc_turn_value             = 0x00;
    _loc_drive_value            = 0x00;


    if(_master_controller == nullptr)
        _master_controller = this;
}

ControllerStatus::~ControllerStatus()
{

}

bool ControllerStatus::PushByte(uchar byte)
{
    if(_write_cnt > 0)
    {
        SetByte(_addr, byte);
        _write_cnt --;
        _addr++;
        return true;
    }

    switch (_encoding_state)
    {
    case 0: // read 1 byte command
        _command = byte;

        switch(_command)
        {
            case RESTART_CMD    :  _restart = true;     break;
            case SET_AS_MASTER  :  SetAsMaster();       break;
            case SET_SIM_ON     :  SetSimMode(true);    break;
            case SET_SIM_OFF    :  SetSimMode(false);   break;
        default:
            _encoding_state = 1;
            break;
        };

        break;

    case 1: // read addr
        _addr = byte;
        if(IS_READ_CMD(_command))
        {
            setOutBuffer(_addr, _command >> 4);
        }
        if (IS_WRITE_CMD(_command))
        {
            _write_cnt = _command;
        }

        _encoding_state = 0;
    default:
        break;
    }

    return true;
}

uchar ControllerStatus::GetByte(uchar addr)
{
    uchar * byteAddr = GetBytePtr( addr );
    if(byteAddr == nullptr) return 0x00;
    return *byteAddr;
}

uchar *ControllerStatus::GetBytePtr(uchar addr)
{
    switch (addr)
    {
        case  ADDR_LOC_DRIVE_MODEL:         updateDrivelModel(); return &_loc_driver_model;
        case  ADDR_LOC_LEFT_ENGINE_SPEED:   return &_loc_left_engine_speed;
        case  ADDR_LOC_RIGHT_ENGINE_SPEED:  return &_loc_right_engine_speed;
        case  ADDR_LOC_LEFT_ENGINE_CURENT:  return &_loc_left_engine_curent;
        case  ADDR_LOC_RIGHT_ENGINE_CURENT: return &_loc_right_engine_curent;
        case  ADDR_LOC_GEAR_VALUE:          return &_loc_gear_value;
        case  ADDR_LOC_TURN_VALUE:          return &_loc_turn_value;
        case  ADDR_LOC_DRIVE_VALUE:         return &_loc_drive_value;

        case  ADDR_MTR_DRIVE_MODEL:         _master_controller->updateDrivelModel(); return &_master_controller->_loc_driver_model;
        case  ADDR_MTR_LEFT_ENGINE_SPEED:   return &_master_controller->_loc_left_engine_speed;
        case  ADDR_MTR_RIGHT_ENGINE_SPEED:  return &_master_controller->_loc_right_engine_speed;
        case  ADDR_MTR_LEFT_ENGINE_CURENT:  return &_master_controller->_loc_left_engine_curent;
        case  ADDR_MTR_RIGHT_ENGINE_CURENT: return &_master_controller->_loc_right_engine_curent;
        case  ADDR_MTR_GEAR_VALUE:          return &_master_controller->_loc_gear_value;
        case  ADDR_MTR_TURN_VALUE:          return &_master_controller->_loc_turn_value;
        case  ADDR_MTR_DRIVE_VALUE:         return &_master_controller->_loc_drive_value;

        case  ADDR_SYS_CONNECTIONS_CNT:     return &_sys_connections_cnt;
        case  ADDR_SYS_CONNECTIONS_ERR_CNT: return &_sys_connections_err_cnt;

    default: return nullptr;
    }

    return nullptr;
}

void ControllerStatus::SetByte(uchar addr, uchar val)
{
    // can write only from [ 0x00 - 0x07 ]
    if(addr >= 0x8) return;

    uchar * byteAddr = GetBytePtr( addr );
    if(byteAddr == nullptr) return;

    *byteAddr = val;

    if( addr == ADDR_LOC_DRIVE_MODEL )
        set_loc_drive_model( _loc_driver_model );
}

bool ControllerStatus::IsMaster()
{
    return (_master_controller == this);
}

void ControllerStatus::SetAsMaster()
{
    _master_controller = this;
    updateDrivelModel();
}

void ControllerStatus::SetSimMode(bool e)
{
    ___is_simulation = e ? 0x01 : 0x00;
    updateDrivelModel();
}

bool ControllerStatus::Restart()
{
    return _restart;
}

uchar ControllerStatus::SendDataLength()
{
    return _out_buffer_data_cnt;
}

uchar *ControllerStatus::FlushSendData()
{
    _out_buffer_data_cnt = 0;
    return _out_buffer;
}

void ControllerStatus::SetEmergencyBreak(bool v)
{
    ___is_emergensy_break = v;
    updateDrivelModel();
}

void ControllerStatus::setOutBuffer(uchar addr, uchar vl)
{
    if(vl > 0x11) vl = 0x11;

    _out_buffer_data_cnt = vl;
    for(int i=0; i<vl; i++)
    {
        _out_buffer[i] = GetByte( addr + i);
    }
}

void ControllerStatus::updateDrivelModel()
{
    _loc_driver_model = get_loc_drive_model();
}

void ControllerStatus::set_loc_drive_model(uchar val)
{
    _loc_driver_model = val;

    ___is_simulation = (val & 0x01);
    ___drive_model = (val & 0x0C);
    ___is_emergensy_break = (val & 0x80);
}

uchar ControllerStatus::get_loc_drive_model()
{
    return ___is_emergensy_break | ___is_simulation | ___drive_model | ( IsMaster() ? 0x02 : 0x00 );
}

uchar   ControllerStatus::_sys_connections_cnt          = 0x00;
uchar   ControllerStatus::_sys_connections_err_cnt      = 0x00;


ControllerStatus    * ControllerStatus::_master_controller = nullptr;


ControllerStatusWrapper::ControllerStatusWrapper(ControllerStatus * st) :
    _status(st)
{}

ControllerStatusWrapper::~ControllerStatusWrapper()
{
    if(_status != nullptr)
        delete _status;
}

bool ControllerStatusWrapper::ShouldRestart()
{
    return _status->Restart();
}

bool ControllerStatusWrapper::BrakeOn()
{
    uchar b = _status->GetByte(ADDR_LOC_DRIVE_MODEL);
    return ((b & 0x0C) == 0x0C);
}

bool ControllerStatusWrapper::EmergencyBrakeOn()
{
    uchar b = _status->GetByte(ADDR_LOC_DRIVE_MODEL);
    return ((b & 0x10) == 0x80);
}

bool ControllerStatusWrapper::TankModel()
{
    uchar b = _status->GetByte(ADDR_LOC_DRIVE_MODEL);
    return ((b & 0x0C) == 0x08);
}

bool ControllerStatusWrapper::CarModel()
{
    uchar b = _status->GetByte(ADDR_LOC_DRIVE_MODEL);
    return ((b & 0x0C) == 0x04);
}

float ControllerStatusWrapper::LeftMotors()
{
    char b = (char)_status->GetByte(ADDR_LOC_LEFT_ENGINE_SPEED);
    if(b > 100) b = 100;
    if(b < -100) b = -100;

    return float(b)/100.0f;
}

float ControllerStatusWrapper::RightMotors()
{
    char b = (char)_status->GetByte(ADDR_LOC_RIGHT_ENGINE_SPEED);
    if(b > 100) b = 100;
    if(b < -100) b = -100;

    return float(b)/100.0f;
}

float ControllerStatusWrapper::Turn()
{
    char b = (char)_status->GetByte(ADDR_LOC_TURN_VALUE);
    if(b > 100) b = 100;
    if(b < -100) b = -100;

    return float(b)/100.0f;
}

float ControllerStatusWrapper::Drive()
{
    char b = (char)_status->GetByte(ADDR_LOC_DRIVE_VALUE);
    if(b > 100) b = 100;
    if(b < -100) b = -100;

    return float(b)/100.0f;
}

float ControllerStatusWrapper::Gear()
{
    uchar b = _status->GetByte(ADDR_LOC_GEAR_VALUE);
    return float(b)/256.0f;
}

bool ControllerStatusWrapper::IsSimulationMode()
{
    uchar b = _status->GetByte(ADDR_LOC_DRIVE_MODEL);
    return ((b & 0x01) == 0x01);
}

bool ControllerStatusWrapper::IsThisMaster()
{
    return _status->IsMaster();
}

void ControllerStatusWrapper::SetCurrentValueLeft(float v)
{
    uchar uv = uchar(256.0f*v);
    _status->SetByte(ADDR_LOC_LEFT_ENGINE_CURENT, uv);
}

void ControllerStatusWrapper::SetCurrentValueRight(float v)
{
    uchar uv = uchar(256.0f*v);
    _status->SetByte(ADDR_LOC_RIGHT_ENGINE_CURENT, uv);
}

void ControllerStatusWrapper::SetEmergencyBreak()
{
    _status->SetEmergencyBreak(true);
}

ControllerStatus *ControllerStatusWrapper::Status()
{
    return _status;
}
