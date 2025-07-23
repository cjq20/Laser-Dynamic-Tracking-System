#ifndef __PID_H
#define	__PID_H

#include "sys.h"

#define	D_KP 0.02
#define	D_KI 0.01
#define	D_KD 0.01

typedef struct
{
	//相关速度PID参数
	float Kp;
	float Ki;
	float Kd;
    
	int Target_Val;//目标值					
	int Last_Err;//上次误差
    int AddErr;
}PID_Struct;

void PID_Init(PID_Struct* PIDX);
int PID_realize(PID_Struct* PIDX,int Actual_val);
#endif
