#include "Hall.h"
/**
 * @brief 把TIM2初始化为编码器接口模式
 * @param psc 预分频系数
 * @param arr 自动重装载值
 * @retval None
 */
void Encoder_Init_TIM2(uint16_t psc,uint16_t arr)	
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //NVIC_InitTypeDef  NVIC_InitStructure;



    //使能定时器2的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //使能PB端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);



    //端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    //浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    //根据设定参数初始化GPIOB
    GPIO_Init(GPIOA, &GPIO_InitStructure);



    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    // 预分频器
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    //设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_Period = arr;
    //选择时钟分频：不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //TIM向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);



    //使用编码器模式3
    TIM_EncoderInterfaceConfig(TIM2, \
                               TIM_EncoderMode_TI12, \
                               TIM_ICPolarity_Rising, \
                               TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);



    //清除TIM的更新标志位
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    //Reset counter
    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief 单位时间读取编码器计数
 * @param TIMX 定时器
 * @retval 速度值	是编码器返回的脉冲
 */
int Read_Encoder()
{
    	int Encoder_TIM;
        Encoder_TIM= (short)TIM2 -> CNT;
        //Encoder_TIM= (int)((int16_t)(TIM4->CNT));;
        TIM2 -> CNT=0;
   	    return Encoder_TIM;
}
