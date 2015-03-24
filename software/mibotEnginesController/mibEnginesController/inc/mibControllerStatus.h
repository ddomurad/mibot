#ifndef COMMANDINTERPRETED_H
#define COMMANDINTERPRETED_H

#include <QtCore>

namespace mibot
{

/** code definition *******************
 *
 *  CMD's:
 *
 *  WRITE N BYTE:   0x0N
 *  READ N BYTE:    0xN0
 *
 *  RST.CTRL:       0xAA
 *  SET.MASTER.ON:  0xBB
 *  SET.SIM.ON:     0xCC
 *  SET.SIM.OFF:    0xDD
 *
 *
 *  REG's:
 * // values for channels
 *  0x00    : Drive model
 *  0x01    : Left engine speed ( -100 .. +100 ) [ Tank mode ]
 *  0x02    : Right engine speed ( -100 .. +100 ) [ Tank mode ]
 *  0x03    : Left engine current ( 0 .. 256 ) ( read only )
 *  0x04    : Right engine current ( 0 .. 256 ) ( read only )
 *  0x05    : Gear value ( 0 .. 256 ) [ Car mode ]
 *  0x06    : Turn value ( -100 .. +100 ) [ Car mode ]
 *  0x07    : Drive value ( -100 .. +100 ) [ Car mode ]
 *
 * // masters values
 *
 *  0x08    : Drive model
 *  0x09    : Left engine speed ( -100 .. +100 ) [ Tank mode ]
 *  0x0A    : Right engine speed ( -100 .. +100 ) [ Tank mode ]
 *  0x0B    : Left engine current ( 0 .. 256 )
 *  0x0C    : Right engine current ( 0 .. 256 )
 *  0x0D    : Gear value ( 0 .. 256 ) [ Car mode ]
 *  0x0E    : Turn value ( -100 .. +100 ) [ Car mode ]
 *  0x0F    : Drive value ( -100 .. +100 ) [ Car mode ]
 *
 * //system status ( read only )
 *
 *  0x10    : Connections count
 *  0x11    : Connections errors count
 *
 * Drive model:
 * E000 DDMS:
 *  S - simulation mode
 *  M - master mode
 *  DD - driver type ( 01 - car model, 10 - tank model , 11 - break)
 *  E - Emergency Brake
 */

#define RESTART_CMD     0xAA
#define SET_AS_MASTER   0xBB
#define SET_SIM_ON      0xCC
#define SET_SIM_OFF     0xDD

#define IS_READ_CMD(X) ( (X & 0x0F) == 0x00 )
#define IS_WRITE_CMD(X) ( (X & 0xF0) == 0x00 )

#define ADDR_LOC_DRIVE_MODEL            0x00
#define ADDR_LOC_LEFT_ENGINE_SPEED      0x01
#define ADDR_LOC_RIGHT_ENGINE_SPEED     0x02
#define ADDR_LOC_LEFT_ENGINE_CURENT     0x03
#define ADDR_LOC_RIGHT_ENGINE_CURENT    0x04
#define ADDR_LOC_GEAR_VALUE             0x05
#define ADDR_LOC_TURN_VALUE             0x06
#define ADDR_LOC_DRIVE_VALUE            0x07

#define ADDR_MTR_DRIVE_MODEL            0x08
#define ADDR_MTR_LEFT_ENGINE_SPEED      0x09
#define ADDR_MTR_RIGHT_ENGINE_SPEED     0x0A
#define ADDR_MTR_LEFT_ENGINE_CURENT     0x0B
#define ADDR_MTR_RIGHT_ENGINE_CURENT    0x0C
#define ADDR_MTR_GEAR_VALUE             0x0D
#define ADDR_MTR_TURN_VALUE             0x0E
#define ADDR_MTR_DRIVE_VALUE            0x0F

#define ADDR_SYS_CONNECTIONS_CNT        0x10
#define ADDR_SYS_CONNECTIONS_ERR_CNT    0x11

class ControllerStatus
{
public:
    ControllerStatus();
    ~ControllerStatus();

    bool PushByte(uchar byte);

    uchar GetByte(uchar addr);
    uchar *GetBytePtr(uchar addr);
    void  SetByte(uchar addr, uchar val);

    bool  IsMaster();
    void  SetAsMaster();

    void  SetSimMode(bool);

    bool  Restart();

    uchar SendDataLength();
    uchar *FlushSendData();

    void SetEmergencyBreak(bool);

private:
    uchar  _write_cnt;
    static ControllerStatus    * _master_controller;
    void    setOutBuffer(uchar addr, uchar vl);
    void    updateDrivelModel();

    uint    _encoding_state;
    uchar   _command;
    uchar   _addr;
    bool    _restart;

    uchar   _out_buffer_data_cnt;
    uchar   _out_buffer[0x12];

    // local vairables
    void    set_loc_drive_model(uchar val);
    uchar   get_loc_drive_model();

    uchar   ___drive_model;
    uchar   ___is_simulation;
    uchar   ___is_emergensy_break;

    uchar   _loc_driver_model;
    uchar   _loc_left_engine_speed;
    uchar   _loc_right_engine_speed;
    uchar   _loc_left_engine_curent;
    uchar   _loc_right_engine_curent;
    uchar   _loc_gear_value;
    uchar   _loc_turn_value;
    uchar   _loc_drive_value;

    // system varaibles
    static uchar  _sys_connections_cnt;
    static uchar  _sys_connections_err_cnt;
};

class ControllerStatusWrapper
{
public:
    ControllerStatusWrapper(ControllerStatus *);
    ~ControllerStatusWrapper();

    bool ShouldRestart();

    bool BrakeOn();
    bool EmergencyBrakeOn();
    bool TankModel();
    bool CarModel();

    float LeftMotors();
    float RightMotors();

    float Turn();
    float Drive();
    float Gear();

    bool IsSimulationMode();
    bool IsThisMaster();

    void SetCurrentValueLeft(float v);
    void SetCurrentValueRight(float v);

    void SetEmergencyBreak();

    ControllerStatus * Status();
private:
    ControllerStatus * _status;
};

}
#endif // COMMANDINTERPRETED_H


