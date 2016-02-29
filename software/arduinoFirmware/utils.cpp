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

