#include "utils.h"
#include <NewPing.h>


#include "config.h"
#include "us.h"

NewPing sonar1(US1_TRIG_PIN, US1_ECHO_PIN, US1_MAX_DISTANCE);
NewPing sonar2(US2_TRIG_PIN, US2_ECHO_PIN, US2_MAX_DISTANCE);

int sonar_index = 0;

bool ReadSonar(NewPing *sonar, int *d, int * sn)
{
  *sn = sonar_index;
  int uS = sonar->ping_cm();
  if(uS == 0)
  uS = 201;
  *d = uS;
  sonar_index = 0;//sonar_index == 0 ? 1 : 0;

  return true;
}

bool ReadUS(int *d, int *sn)
{
  if(sonar_index == 0)
   return ReadSonar(&sonar1, d, sn);

  if(sonar_index == 1)
   return ReadSonar(&sonar2, d, sn);

   return false;
}

void ReadAndSendUS()
{
  int d, sn;
  if(ReadUS(&d, &sn))
  {
    SendData<int>("US",sn,d);
  }
}
