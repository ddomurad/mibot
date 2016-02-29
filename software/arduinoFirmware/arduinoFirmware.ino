#include <TimerOne.h>

#include <NewPing.h>
#include <Wire.h>

#include "config.h"
#include "utils.h"
#include "analog.h"
#include "lsm303.h"
#include "us.h"
#include "PiezzoSignal.h"

char trig = 1;
unsigned long timeA = 0;
unsigned long timeB = 0;
unsigned long timeC = 0;


void setup()
{
  InitGSerial(GSERIAL, GSERIAL_BAUD);

  InitAnalog();
  LSM_Init();
  InitPiezzo();
  PiezzoState(true);
}

void loop()
{

  /*ReadAndSendAnalog();
  LSM_ReadAccAndSend();
  LSM_ReadMagAndSend();
  ReadAndSendUS();*/
}


