#ifndef __CONFIG_H
#define __CONFIG_H

#include "Arduino.h"
#include "HardwareSerial.h"

// configs
//#define ENABLE_TRACE
//#define ENABLE_TRACE2
//#define ENABLE_TRACE3

#define GSERIAL_BAUD 19200
#define GSERIAL &Serial

#define __ERROR_TIMER   1000

#define LSM_ACC_ADDR               0b0011000 //accelerometer address
#define LSM_ACC_CTRL_REG1_A_ADDR   0b00100000 //CTRL_REG1_A_ADDR
#define LSM_ACC_OUT_ADDR           0b10101000 //first output_register ( OUT_X_L_A )

#define LSM_ACC_PM    0x20 //normal mode
#define LSM_ACC_DR    0x00 //50 hz
#define LSM_ACC_XYZ   0x07 //enable x,y,z

#define LSM_MAG_ADDR       0b0011110//magnetometer address
#define LSM_MAG_CRA_REG_M_ADDR   0x00
#define LSM_MAG_CRB_REG_M_ADDR   0x01
#define LSM_MAG_MR_REG_M_ADDR    0x02
#define LSM_MAG_OUT_ADDR         0x03 //first output register (OUT_X_H_M)

#define LSM_MAG_DO    0x10 // 15 Hz
#define LSM_MAG_GN    0x20 // +- 1.3 
#define LSM_MAG_MD    0x00 //Continuos-sonverion mode

// post config
#define LSM_ACC_CTRL_REG1_A  ( LSM_ACC_PM | LSM_ACC_DR | LSM_ACC_XYZ )

#define LSM_MAG_CRA_REG_M ( LSM_MAG_DO )
#define LSM_MAG_CRB_REG_M ( LSM_MAG_GN )
#define LSM_MAG_MR_REG_M  ( LSM_MAG_MD )

#define US1_TRIG_PIN 53
#define US2_TRIG_PIN 52
#define US1_ECHO_PIN 51
#define US2_ECHO_PIN 50

#define US1_MAX_DISTANCE 200
#define US2_MAX_DISTANCE 200

#define PIEZZO_1_PIN 31
#define PIEZZO_2_PIN 30
#endif

