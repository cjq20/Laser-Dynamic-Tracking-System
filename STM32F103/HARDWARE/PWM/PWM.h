#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

#define   PWMA_IN1  TIM3->CCR1//A1
#define   PWMA_IN2  TIM3->CCR2//A2


void PWM_Int(u16 arr,u16 psc);
void Set_PWM(int Vertical,int Level);
#endif
