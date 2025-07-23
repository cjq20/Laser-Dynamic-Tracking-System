#include "control.h"
#include "PWM.h"

/*
530     337    130垂直
410     280    130水平
*/
PID_Struct PID_Vertical,PID_Level;
int Vertical_PWM=337,Level_PWM=280;
char PID_TAB=0,PFINT_TAB=0,KEY_TAB=0;
//控制初始化
void Control_Init(void)
{
    Control_Init_GPIO();
    PWM_Int(4999,287); 
    PID_Init(&PID_Vertical);
    PID_Init(&PID_Level);
    PID_Set_Target(120,120);
    Astrict_IN();
    Set_PWM_IN(Vertical_PWM,Level_PWM);
    RED_IN=0;
    BLOCK_IN=0;
}


void Control_Init_GPIO(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5

}
void Set_PWM_IN(int Vertical,int Level)
{
    Set_PWM(Vertical,Level);
}

void Astrict_IN(void)
{
if(Vertical_PWM>620)
    Vertical_PWM=620;
if(Vertical_PWM<130)
    Vertical_PWM=130;

if(Level_PWM>620)
    Level_PWM=620;
if(Level_PWM<130)
    Level_PWM=130;   
}

void PID_Set_Target(int Vertical,int Level)
{
PID_Vertical.Target_Val=Vertical;
PID_Level.Target_Val=Level;    
}
void Set_V(int Vertical)
{
PWMA_IN1=Vertical;
}

void Set_L(int Level)
{
PWMA_IN2=Level;
}
