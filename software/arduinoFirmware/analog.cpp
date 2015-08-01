#include "config.h"
#include "utils.h"
#include "analog.h"

#define ANALOG_CHANNELS 5
unsigned char _last_analog_channels_state_flags[ANALOG_CHANNELS];
unsigned int _read_from_channels_state[ANALOG_CHANNELS];

const char *_analog_labels = "A1\0A2\0A3\0A4\0A5\0";
void InitAnalog()
{
  for(int i=0;i<ANALOG_CHANNELS;i++) 
    _read_from_channels_state[i] = 0;    
}

void ReadAndSendAnalog()
{
  for(int i=0;i<ANALOG_CHANNELS;i++)
  {
    unsigned char s = GetStateFlags(F_USE_ANALOG1 + i);
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
