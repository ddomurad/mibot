#include "config.h"
#include "utils.h"
#include "analog.h"

#define ANALOG_CHANNELS 10
unsigned char _last_analog_channels_state_flags[ANALOG_CHANNELS];
unsigned int _read_from_channels_state[ANALOG_CHANNELS];

const char *_analog_labels = "A0\0A1\0A2\0A3\0A4\0A5\0A6\0A7\0A8\0A9\0";
char _analog_flags[ANALOG_CHANNELS] = {50,50, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

void InitAnalog()
{
  for(int i=0;i<ANALOG_CHANNELS;i++) 
    _read_from_channels_state[i] = 0;    
}

void SetAnalog(int channel, bool state)
{
  if(channel >= ANALOG_CHANNELS)
    return;

  if(channel < 0)
    return;

  _analog_flags[channel] = state == true ? 50 : 0xff;
}

void ReadAndSendAnalog()
{
  for(int i=0;i<ANALOG_CHANNELS;i++)
  {
    unsigned char s = _analog_flags[i];
    if(s == 0xFF) continue;
    
    if(_last_analog_channels_state_flags[i] != s)
    {
      _last_analog_channels_state_flags[i] = s;
      _read_from_channels_state[i] = 0;
    }
    
    if(_read_from_channels_state[i]== 0)
    {
      _read_from_channels_state[i] = _last_analog_channels_state_flags[i];
  
      int r = analogRead(i);
      SendData<int>(_analog_labels + i*3, r);
    }else
    {
      _read_from_channels_state[i] --;
    }
  }
}
