#include <TimerThree.h>
#include <TimerOne.h>
#include <NewPing.h>
#include <Wire.h>

#include "config.h"
#include "utils.h"
#include "analog.h"
#include "lsm303.h"
#include "us.h"
#include "PiezzoSignal.h"
#include "cmdReader.h"
#include "MotorsLock.h"

void setup()
{
  InitGSerial(GSERIAL, GSERIAL_BAUD);
  InitMotorsLock();
  InitAnalog();
  LSM_Init();
  InitPiezzo();
}

void loop()
{
  ReadCommands();
  ReadAndSendAnalog();
  ReadAndSendUS();
  LSM_ReadAccAndSend();
  LSM_ReadMagAndSend();
}


