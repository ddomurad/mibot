#include "config.h"
#include "utils.h"
#include <Arduino.h>

HardwareSerial* __global_serial;
void InitGSerial(HardwareSerial* s, int buad)
{
  __global_serial = s;
  __global_serial->begin(buad);
}

HardwareSerial* GSerial()
{
  return __global_serial;
}

void __trace_header(int line, const char* funct)
{
     __global_serial->print("TRACE: [");
     __global_serial->print(funct);
     __global_serial->print(":");
     __global_serial->print(line);
     __global_serial->print("] ");
}

void __trace_prt(char * msg, int line_num, const char* function)
{
     __trace_header(line_num, function);
     __global_serial->println(msg);
}

void __handle_error(char * msg,int code, int line_num,const char* function)
{
  while(true)
  {
     __global_serial->print("ERROR: [");
     __global_serial->print(function);
     __global_serial->print(":");
     __global_serial->print(line_num);
     __global_serial->print("] ");
     __global_serial->println(msg);
     
    delay(__ERROR_TIMER);
  }
}

char __FLAGS[FLAGS_CNT];

void InitStateFlags()
{
  for(int i=0;i<FLAGS_CNT;i++)
    __FLAGS[i] = 0;
    
  __FLAGS[F_USE_ANALOG1] = 0xFF;
  __FLAGS[F_USE_ANALOG2] = 0xFF;
  __FLAGS[F_USE_ANALOG3] = 0xFF;
  __FLAGS[F_USE_ANALOG4] = 0xFF;
  __FLAGS[F_USE_ANALOG5] = 0xFF;
}

void ReadStateFlags()
{
  if(GSerial()->available() >= 0)
  {
    char r = GSerial()->read();
    delay(100);
    if(r=='>') // write
    {
      char addr = GSerial()->read() - '0';
      char value = GSerial()->read();
      if(addr >= 0 && addr < FLAGS_CNT)
        SetStateFlags(addr, value);
    }else if(r=='<') // read
    {
      char addr = GSerial()->read() - '0';
      if(addr >= 0 && addr < FLAGS_CNT)
        GSerial()->println(GetStateFlags(addr));
      else
        GSerial()->println("ADDR ERROR");
    }
  }
}

void SetStateFlags(char addr, char state)
{
  __FLAGS[addr] = state;
}

char GetStateFlags(char addr)
{
  return __FLAGS[addr];
}
