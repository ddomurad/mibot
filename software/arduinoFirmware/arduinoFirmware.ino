#include <Wire.h>
#include "config.h"
#include "utils.h"
#include "analog.h"
#include "lsm303.h"
#include "gps.h"

void setup()
{
  InitGSerial(GSERIAL, GSERIAL_BAUD);
  InitStateFlags();
  InitAnalog();
  LSM_Init();
  GPS_Init();
}

void loop()
{
  ReadStateFlags();
 
  ReadAndSendAnalog();
 
  if(GetStateFlags(F_USE_ACC) == '1')
      LSM_ReadAccAndSend();
    
  if(GetStateFlags(F_USE_MAG) == '1')
      LSM_ReadMagAndSend();
      
  if(GetStateFlags(F_USE_GPS) == '1')
      GPS_ReadAndSend();
      
}
