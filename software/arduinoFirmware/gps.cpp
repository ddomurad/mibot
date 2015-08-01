#include "config.h"
#include "gps.h"
#include "utils.h"
#include "HardwareSerial.h"

HardwareSerial* __gps_serial;

#define GPS_NMEA_SIZE 5
#define GPS_LINE_MAX_SIZE 255

char __readed_NMEA[GPS_LINE_MAX_SIZE + 1];
char __NMEA_ptr_offset = 0;

bool IsNMEA_GPGGA();
bool IsNMEA_GPVTG();

void GPS_Init()
{
    TRACE_LINE( __gps_serial = GPS_SERIAL );
    if( __gps_serial != GSerial())
          TRACE_LINE( __gps_serial->begin(GPS_SERIAL_BAUD) );
              
    for(int i=0;i<GPS_LINE_MAX_SIZE;i++)
      __readed_NMEA[i] = 0;
}

void GPS_ReadAndSend()
{  
  while(__gps_serial->available() > 0)
  {
    char r = (char)__gps_serial->read();
    if(r == '$' || r == '\n' || r == '\r')
    {
      __NMEA_ptr_offset = 0;
      
      if(r == '\n')
      {
        if(IsNMEA_GPGGA() || IsNMEA_GPVTG())
        {
          SendData<char*>("GPS", __readed_NMEA);
        }
      }
    }else
    {
      if(__NMEA_ptr_offset < GPS_LINE_MAX_SIZE - 1)
      {
        __readed_NMEA[__NMEA_ptr_offset] = r;
        __readed_NMEA[__NMEA_ptr_offset + 1] = 0;
        __NMEA_ptr_offset++;
      }
    }
  }
}


bool IsNMEA_GPGGA()
{
  return __readed_NMEA[0] == 'G' 
      && __readed_NMEA[1] == 'P'
      && __readed_NMEA[2] == 'G'
      && __readed_NMEA[3] == 'G'
      && __readed_NMEA[4] == 'A';
}

bool IsNMEA_GPVTG()
{
  return __readed_NMEA[0] == 'G' 
      && __readed_NMEA[1] == 'P'
      && __readed_NMEA[2] == 'V'
      && __readed_NMEA[3] == 'T'
      && __readed_NMEA[4] == 'G';
}
