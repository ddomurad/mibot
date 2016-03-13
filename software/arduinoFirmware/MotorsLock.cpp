#include "config.h"
#include <TimerThree.h>

#include "MotorsLock.h"
#include "PiezzoSignal.h"

void testLock();

int _loop_cnt = 0;
bool locked = false;

void InitMotorsLock()
{
  pinMode(MOTORS_LOCK_PIN, OUTPUT);
  digitalWrite(MOTORS_LOCK_PIN, LOW);
  
  Timer3.initialize();
  Timer3.attachInterrupt(testLock,100000);

  locked = true;
}

void ResetMotorstLock()
{
 _loop_cnt = 0; 
 if(locked == true)
 {
  digitalWrite(MOTORS_LOCK_PIN, HIGH);
   locked = false;
   PiezzoState(false);
 }
}


void testLock()
{
  _loop_cnt++;
  if(_loop_cnt > 5 && locked == false)
    {
      digitalWrite(MOTORS_LOCK_PIN, LOW);
      locked = true;
      PiezzoState(true);
    }
    if(_loop_cnt > 20 && locked == true)
    {
      PiezzoState(false);
      _loop_cnt = 0;
    }
}
