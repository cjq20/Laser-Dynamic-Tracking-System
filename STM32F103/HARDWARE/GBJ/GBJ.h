#ifndef __GBJ_H
#define __GBJ_H
#include "sys.h"	 

#define PWMA   TIM1->CCR1  //PA8  PWMA  TIM1_CH1
#define AIN2   PBout(15)
#define AIN1   PBout(14)

void Motor_PWM_Init(u16 arr,u16 psc);
void Motor_SetSpeed(u8 mode ,u16 speed);

#endif
