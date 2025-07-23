#include "PID.h"

int	D_KA=120;

void PID_Init(PID_Struct* PIDX)
{
    PIDX->Kp=D_KP;
    PIDX->Ki=D_KI;
    PIDX->Kd=D_KD;
    PIDX->Target_Val=0;
    PIDX->Last_Err=0;
    PIDX->AddErr=0;
}

int PID_realize(PID_Struct* PIDX,int Actual_val)
{
	/*计算目标值与实际值的误差*/
	int Err =PIDX->Target_Val - Actual_val;
	int Out_Val;
	/*积分项*/
	PIDX->AddErr += Err;
    //限幅
    if(PIDX->AddErr > D_KA)
        PIDX->AddErr=D_KA;
    else if(PIDX->AddErr < -D_KA)
        PIDX->AddErr=-D_KA;
    

	/*PID算法实现*/
	Out_Val = PIDX->Kp * Err + PIDX->Ki * PIDX->AddErr + PIDX->Kp * (Err - PIDX->Last_Err);

	/*误差传递*/
	PIDX->Last_Err = Err;

	/*返回当前实际值*/
	return Out_Val;
}


