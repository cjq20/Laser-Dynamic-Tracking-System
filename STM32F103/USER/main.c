#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "pwm.h"
#include "key.h"
#include "control.h"
int Target_x_Data[20];
int Target_y_Data[20];
/*
Vertical    PA6     上大下小
Level       PA7     右小左大
G6          PD8
G7          PD9
*/

void Coord_Dispose(int x1,int y1,int x2,int y2,int Star)   
{
int i=0;
int x_i,y_i;
x_i=(x2-x1)*200;
y_i=(y2-y1)*200;   
for(i=0;i<5;i++)
{
Target_x_Data[i+Star]=x1+i*x_i/1000;  
Target_y_Data[i+Star]=y1+i*y_i/1000;     
}
}

void Coord_Dispose_2(int x1,int y1,int x2,int y2,int x3, int y3,int x4,int y4)
{
Coord_Dispose(x1,y1,x2,y2,0);
Coord_Dispose(x2,y2,x3,y3,5);
Coord_Dispose(x3,y3,x4,y4,10);
Coord_Dispose(x4,y4,x1,y1,15);    
}

 int main(void)
{
    int filter_i,Plan_i;
    int Red_x=120,Red_y=120;
    int Block_x1=120,Block_y1=120,Block_x2=120,Block_y2=120,Block_x3=120,Block_y3=120,Block_x4=120,Block_y4=120;
    int Target_x=120,Target_y=120;
    int FLAG_X,FLAG_Y;
    u8 Key_Val,Procedure_TAB,KEY_SWITCH=1,PID_SWITCH,Procedure_STAR=1,PID_USART;
    int temp_i;
    
    
	delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组
    uart_init(115200);
    USART2_Init(9600);
    LED_Init();
    KEY_Init();
    Control_Init();
    TIM5_Int_Init(999,719);
    PID_Set_Target(Target_y,Target_x);

  while (1)
 {     

        /*按键标记每0.05s一次*/
        if(KEY_TAB==1)
        {
            if(KEY_SWITCH==1)
            {
            Key_Val=KEY_Scan();
            if(Key_Val!=0)
            {
            Procedure_TAB=Key_Val;
            //printf("k=%d",Key_Val);                   
            }
            }
            KEY_TAB=0;
        }
        
        if(Procedure_TAB==6)//复位
        {
            if(Procedure_STAR==1)
            {   
            Procedure_STAR=0;
            KEY_SWITCH=0;//关掉切换按键
            PID_SWITCH=1;//打开PID  
            RED_IN=1;    
            Vertical_PWM=337;
            Level_PWM=280;
            delay_ms(500);
            Coord_Dispose_2(120,120,120,120,120,120,120,120);//右上角顶点
            Plan_i=0;
            /*    
            KEY_SWITCH=1;//开启切换按键
            Procedure_TAB=0;//结束程序
            Procedure_STAR=1;
            */
            }
        }
        
        if(Procedure_TAB==7)
        {
            Procedure_TAB=0;
            /*
            if(Procedure_STAR==1)
            {
            Procedure_STAR=0;
            KEY_SWITCH=0;//关掉切换按键
            PID_SWITCH=0;//关闭PID
            RED_IN=0;//关闭红输入
            BLOCK_IN=1;//打开黑输入
            printf("7");
            }
            */
        }
        
        /*PID标记每0.05s一次*/
        if(PID_TAB==1)
        {
            
            if(PID_SWITCH==1)
            {
            Target_x= Target_x_Data[Plan_i];
            Target_y= Target_y_Data[Plan_i];
            
            if(PID_USART)
            {
            //printf("PID");    
            PID_USART=0;
                
            if(Red_x - Target_x > 10||Red_x - Target_x < -10)
            {
            Level_PWM-=PID_realize(&PID_Level,Red_x); 
            //printf("L=%d",Level_PWM); 
            Astrict_IN();
            Set_L(Level_PWM);    
            FLAG_X=0;FLAG_Y=0;
            }
            else
            {
            FLAG_X=1;    
            }
            
            if(Red_y - Target_y > 10||Red_y - Target_y < -10)
            {
            Vertical_PWM-=PID_realize(&PID_Vertical,Red_y);
                Astrict_IN();
                Set_V(Vertical_PWM);
            FLAG_X=0;FLAG_Y=0;
            //printf("V=%d",Vertical_PWM);                
            }
            else
            {
            FLAG_Y=1;    
            }
            //printf("V=%d",Vertical_PWM);
            }
            if(FLAG_X==1&&FLAG_Y==1)
            {
                //printf("OK");
            FLAG_X=0;
            FLAG_Y=0;    
            Plan_i++;    
            }
            
            if(Plan_i==20)//程序结束
            {
            Plan_i=0;
            Procedure_STAR=1;
            KEY_SWITCH=1;
            PID_SWITCH=0;//打开PID  
            RED_IN=0; 
            //printf("OK2");    
            }
            }        
        PID_TAB=0;   
        } 
        
        /*串口发送标记每0.5s一次*/
        if(PFINT_TAB==1)
        {
        
        PFINT_TAB=0;            
        }
        
        /*串口接收标记收到DK一次*/
        if(USART_TAB == 1)
        {
            for(filter_i=0;filter_i<50;filter_i++)//滤掉包外的字符
                if(USART_RX_BUF[filter_i]=='R'||USART_RX_BUF[filter_i]=='B'||USART_RX_BUF[filter_i]=='T')
                    break;
            if(USART_RX_BUF[filter_i]=='R')//R开头的包
            {
                if(RED_IN==1)
                {
                filter_i++;
                if(USART_RX_BUF[filter_i]=='X')
                {
                    Red_x = (USART_RX_BUF[filter_i+1]-'0')*100+(USART_RX_BUF[filter_i+2]-'0')*10+(USART_RX_BUF[filter_i+3]-'0');              
                }
                filter_i+=4;
                if(USART_RX_BUF[filter_i]=='Y')
                {
                    Red_y = (USART_RX_BUF[filter_i+1]-'0')*100+(USART_RX_BUF[filter_i+2]-'0')*10+(USART_RX_BUF[filter_i+3]-'0');              
                }
                
                PID_USART=1;
                //printf("X=%d,Y=%d",Red_x,Red_y);
                }
                
            }
            if(USART_RX_BUF[filter_i]=='B')//B开头的包
            {
                if(BLOCK_IN==1)
                {
                filter_i++;
                if(USART_RX_BUF[filter_i]=='X')
                {
                    Block_x1 = (USART_RX_BUF[filter_i+1]-'0')*100+(USART_RX_BUF[filter_i+2]-'0')*10+(USART_RX_BUF[filter_i+3]-'0');              
                }
                filter_i+=4;
                if(USART_RX_BUF[filter_i]=='Y')
                {
                    Block_y1 = (USART_RX_BUF[filter_i+1]-'0')*100+(USART_RX_BUF[filter_i+2]-'0')*10+(USART_RX_BUF[filter_i+3]-'0');              
                }
                filter_i+=4;
                
                if(USART_RX_BUF[filter_i]=='X')
                {
                    Block_x2 = (USART_RX_BUF[filter_i+1]-'0')*100+(USART_RX_BUF[filter_i+2]-'0')*10+(USART_RX_BUF[filter_i+3]-'0');              
                }
                filter_i+=4;
                if(USART_RX_BUF[filter_i]=='Y')
                {
                    Block_y2 = (USART_RX_BUF[filter_i+1]-'0')*100+(USART_RX_BUF[filter_i+2]-'0')*10+(USART_RX_BUF[filter_i+3]-'0');              
                }
                filter_i+=4;
                
                if(USART_RX_BUF[filter_i]=='X')
                {
                    Block_x3 = (USART_RX_BUF[filter_i+1]-'0')*100+(USART_RX_BUF[filter_i+2]-'0')*10+(USART_RX_BUF[filter_i+3]-'0');              
                }
                filter_i+=4;
                if(USART_RX_BUF[filter_i]=='Y')
                {
                    Block_y3 = (USART_RX_BUF[filter_i+1]-'0')*100+(USART_RX_BUF[filter_i+2]-'0')*10+(USART_RX_BUF[filter_i+3]-'0');              
                }
                filter_i+=4;
                
                if(USART_RX_BUF[filter_i]=='X')
                {
                    Block_x4 = (USART_RX_BUF[filter_i+1]-'0')*100+(USART_RX_BUF[filter_i+2]-'0')*10+(USART_RX_BUF[filter_i+3]-'0');              
                }
                filter_i+=4;
                if(USART_RX_BUF[filter_i]=='Y')
                {
                    Block_y4 = (USART_RX_BUF[filter_i+1]-'0')*100+(USART_RX_BUF[filter_i+2]-'0')*10+(USART_RX_BUF[filter_i+3]-'0');              
                }

                Coord_Dispose_2(Block_x1,Block_y1,Block_x2,Block_y2,Block_x3,Block_y3,Block_x4,Block_y4);
                for(temp_i=0;temp_i<20;temp_i++)
                {
                //printf("x=%d,y=%d\n",Target_x_Data[temp_i],Target_y_Data[temp_i]);    
                }
                KEY_SWITCH=1;//关掉切换按键
                BLOCK_IN=0;//打开黑输入
                Procedure_STAR=1;
                Procedure_TAB=0;
                }
                if(USART_RX_BUF[filter_i]=='T')
                printf("%s",USART_RX_BUF);
            }            
        USART_TAB=0;            
        }

 }
}
