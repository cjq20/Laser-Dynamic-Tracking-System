#ifndef __CONTROL_H
#define	__CONTROL_H

#include "sys.h"
#include "PID.h"

extern PID_Struct PID_Vertical,PID_Level;
extern char PID_TAB,PFINT_TAB,KEY_TAB;
extern int Vertical_PWM,Level_PWM;

#define RED_IN      PDout(8)// PB5
#define BLOCK_IN    PDout(9)// PE5	

void Control_Init(void);
void Set_PWM_IN(int Vertical,int Level);
void Astrict_IN(void);
void PID_Set_Target(int Vertical,int Level);
void Control_Init_GPIO(void);
void Set_V(int Vertical);
void Set_L(int Level);
#endif
