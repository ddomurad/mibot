#include <Wire.h>
#include "config.h"
#include "utils.h"
#include "lsm303.h"

char * GetWireError(int code);

void LSM_ACC_INIT_CTRL_REG1_A();

void LSM_MAG_INIT_CRA_REG_M();
void LSM_MAG_INIT_CRB_REG_M();
void LSM_MAG_INIT_MR_REG_M();

void LSM_Init()
{
      TRACE_LINE( Wire.begin() );
    
      TRACE_LINE( LSM_ACC_INIT_CTRL_REG1_A() );

      TRACE_LINE( LSM_MAG_INIT_CRA_REG_M() );
      TRACE_LINE( LSM_MAG_INIT_CRB_REG_M() );
      TRACE_LINE( LSM_MAG_INIT_MR_REG_M() );
}

void LSM_ReadAcc(short *x, short *y, short *z)
{
  TRACE2_PRT("LSM_ReadAcc");
  
  short x_val = 0;
  short y_val = 0;
  short z_val = 0;
  unsigned char *x_prt = (unsigned char*) &x_val;
  unsigned char *y_prt = (unsigned char*) &y_val;
  unsigned char *z_prt = (unsigned char*) &z_val;
  
  TRACE2_LINE( Wire.beginTransmission( LSM_ACC_ADDR ) );
  TRACE2_LINE( Wire.write( LSM_ACC_OUT_ADDR ) );
  TRACE2_LINE( int r1 = Wire.endTransmission() );
  TRACE2_INT( r1 );
  if(r1 != 0) ERR(0, GetWireError(r1));  

  TRACE2_LINE( Wire.requestFrom((int) LSM_ACC_ADDR, 6 ) );
  int i = 1000;
  while (Wire.available() < 6)
    {
      i--;
      if(i <= 0)
        ERR(0, "Can't get LSM_ACC response");
    }
    
  TRACE2_INT(x_prt[0] = Wire.read() );
  TRACE2_INT(x_prt[1] = Wire.read() );
  
  TRACE2_INT(y_prt[0] = Wire.read() );
  TRACE2_INT(y_prt[1] = Wire.read() );
  
  TRACE2_INT(z_prt[0] = Wire.read() );
  TRACE2_INT(z_prt[1] = Wire.read() );
  
  TRACE2_LINE( int r2 = Wire.endTransmission() );
  TRACE2_INT( r2 );
  
  if(r2 != 0) ERR(0, GetWireError(r2));  
  
  *x = x_val;
  *y = y_val;
  *z = z_val;
}

void LSM_ReadMag(short *x, short *y, short *z)
{
  TRACE2_PRT("LSM_ReadMag");
  
  short x_val = 0;
  short y_val = 0;
  short z_val = 0;
  unsigned char *x_prt = (unsigned char*) &x_val;
  unsigned char *y_prt = (unsigned char*) &y_val;
  unsigned char *z_prt = (unsigned char*) &z_val;
  
  TRACE2_LINE( Wire.beginTransmission( LSM_MAG_ADDR ) );
  TRACE2_LINE( Wire.write( LSM_MAG_OUT_ADDR ) );
  TRACE2_LINE( int r1 = Wire.endTransmission() );
  TRACE2_INT( r1 );
  if(r1 != 0) ERR(0, GetWireError(r1));  

  TRACE2_LINE( Wire.requestFrom((int) LSM_MAG_ADDR, 6 ) );
  int i = 1000;
  while (Wire.available() < 6)
    {
      i--;
      if(i <= 0)
        ERR(0, "Can't get LSM_ACC response");
    }
    
  TRACE2_INT(x_prt[1] = Wire.read() );
  TRACE2_INT(x_prt[0] = Wire.read() );
  
  TRACE2_INT(y_prt[1] = Wire.read() );
  TRACE2_INT(y_prt[0] = Wire.read() );
  
  TRACE2_INT(z_prt[1] = Wire.read() );
  TRACE2_INT(z_prt[0] = Wire.read() );
  
  TRACE2_LINE( int r2 = Wire.endTransmission() );
  TRACE2_INT( r2 );
  
  if(r2 != 0) ERR(0, GetWireError(r2));  
  
  *x = x_val;
  *y = y_val;
  *z = z_val;
}

void LSM_ACC_INIT_CTRL_REG1_A()
{
    TRACE_LINE( Wire.beginTransmission( LSM_ACC_ADDR ) )
    TRACE_LINE( Wire.write( LSM_ACC_CTRL_REG1_A_ADDR ) )
    TRACE_LINE( Wire.write( LSM_ACC_CTRL_REG1_A ) )
    TRACE_LINE( int r = Wire.endTransmission() )
    TRACE_INT( r );
    if(r == 0) return; ERR(0, GetWireError(r));
}

void LSM_MAG_INIT_CRA_REG_M()
{
    TRACE_LINE( Wire.beginTransmission( LSM_MAG_ADDR ) )
    TRACE_LINE( Wire.write( LSM_MAG_CRA_REG_M_ADDR ) )
    TRACE_LINE( Wire.write( LSM_MAG_CRA_REG_M ) )
    TRACE_LINE( int r = Wire.endTransmission() )
    TRACE_INT( r );
    if(r == 0) return; ERR(0, GetWireError(r));
}

void LSM_MAG_INIT_CRB_REG_M()
{
    TRACE_LINE( Wire.beginTransmission( LSM_MAG_ADDR ) )
    TRACE_LINE( Wire.write( LSM_MAG_CRB_REG_M_ADDR ) )
    TRACE_LINE( Wire.write( LSM_MAG_CRB_REG_M ) )
    TRACE_LINE( int r = Wire.endTransmission() )
    TRACE_INT( r );
    if(r == 0) return; ERR(0, GetWireError(r));
}

void LSM_MAG_INIT_MR_REG_M()
{
    TRACE_LINE( Wire.beginTransmission( LSM_MAG_ADDR ) )
    TRACE_LINE( Wire.write( LSM_MAG_MR_REG_M_ADDR ) )
    TRACE_LINE( Wire.write( LSM_MAG_MR_REG_M ) )
    TRACE_LINE( int r = Wire.endTransmission() )
    TRACE_INT( r );
    if(r == 0) return; ERR(0, GetWireError(r));
}

char * GetWireError(int code)
{
  switch(code)
  {
    case 1: return "Data too long to fit in transmit buffer ";
    case 2: return "Received NACK on transmit of address";
    case 3: return "Received NACK on transmit of data ";
    case 4: "Other error";
  }
}

void LSM_ReadAccAndSend()
{
    short ax=0,ay=0,az=0;
    LSM_ReadAcc(&ax,&ay,&az);
    SendData<short>("ACC",ax,ay,az);
}

void LSM_ReadMagAndSend()
{
    short mx=0,my=0,mz=0;
    LSM_ReadMag(&mx,&my,&mz);
    SendData<short>("MAG",mx,my,mz);
}
