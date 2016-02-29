#include "config.h"
#include <TimerOne.h>

#include "PiezzoSignal.h"

void callback();

int PERIOD = 1200;

long cnt = 0;

void InitPiezzo()
{
  Timer1.initialize(10000000);
  Timer1.attachInterrupt(callback); 
  
  pinMode(PIEZZO_1_PIN, OUTPUT);
  pinMode(PIEZZO_2_PIN, OUTPUT);
}
void PiezzoState(bool s)
{
  if(s == true)
  {
    Timer1.setPeriod(PERIOD);
    pinMode(PIEZZO_1_PIN, OUTPUT);
    pinMode(PIEZZO_2_PIN, OUTPUT);
  }
  else
  {
    Timer1.setPeriod(10000000);
    pinMode(PIEZZO_1_PIN, INPUT);
    pinMode(PIEZZO_2_PIN, INPUT);
  }
}

int s = 0;
void callback()
{ 
  cnt++;
  if(cnt > 100)
  {
    cnt = 0;
    PERIOD = PERIOD == 1200 ? 900 : 1200;
    Timer1.setPeriod(PERIOD);
   }
  if(s == 0)
  {
     s = 1;
     digitalWrite(PIEZZO_1_PIN,LOW);
    digitalWrite(PIEZZO_2_PIN,HIGH);
  }else
  {
    s = 0;
    digitalWrite(PIEZZO_1_PIN,HIGH);
    digitalWrite(PIEZZO_2_PIN,LOW);
  }
}
