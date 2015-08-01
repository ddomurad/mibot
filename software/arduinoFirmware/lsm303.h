#ifndef __LSM303_H
#define __LSM303_H

void LSM_Init(); 
void LSM_ReadAcc(short *x, short *y, short *z);
void LSM_ReadMag(short *x, short *y, short *z);

void LSM_ReadAccAndSend();
void LSM_ReadMagAndSend();

#endif

