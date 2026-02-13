#ifndef __SERVO_H
#define __SERVO_H

#include "sys.h" 

void Servo_Init(u16 arr,u16 psc);
void Servo_Control(u16 X_Angle,u16 Y_Angle);
void PWM_Xianfu(int X_min,int X_max,int Y_min,int Y_max,int *MOTORX,int *MOTORY);

#endif

