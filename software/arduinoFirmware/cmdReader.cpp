#include "utils.h"
#include "cmdReader.h"
#include "PiezzoSignal.h"
#include "analog.h"

#define RCS_WAIT_FOR_INIT 0x00
#define RCS_WAIT_FOR_W_CMD 0x01
#define RCS_WAIT_FOR_W_VALUE 0x02

int rcState = RCS_WAIT_FOR_INIT;
char wCmd = 0;
char cmdValue = 0;

void _executeCmd()
{
  if(wCmd == CMD_PIEZZO_CTRL)
  {
    PiezzoState(cmdValue == '1');
    return;
  }

  if(wCmd == CMD_ADC_ON)
  {
    SetAnalog(cmdValue - '0', true);
    return;
  }

  if(wCmd == CMD_ADC_OFF)
  {
    SetAnalog(cmdValue - '0', false);
    return;
  }
}


void ReadCommands()
{
  HardwareSerial * serial = GSerial();

  if(!serial->available())
    return;
  
  char r = serial->read();
  if(r == CMD_SYNC)
  {
    TRACE_PRT("sync");
    rcState = RCS_WAIT_FOR_INIT;
    return;
  }
  
  if(rcState == RCS_WAIT_FOR_INIT)
  {
    if(r == CMD_INIT_WRITE)
    {
      TRACE_PRT("write init");
      rcState = RCS_WAIT_FOR_W_CMD;
      return;
    }
  }

  if(rcState == RCS_WAIT_FOR_W_CMD)
  {
    TRACE_PRT("read cmd");
    rcState = RCS_WAIT_FOR_W_VALUE;
    wCmd = r;
    return;
  }

  if(rcState == RCS_WAIT_FOR_W_VALUE)
  {
    TRACE_PRT("read value");
    rcState = RCS_WAIT_FOR_INIT;
    cmdValue = r;
    _executeCmd();
    return;
  }
}
