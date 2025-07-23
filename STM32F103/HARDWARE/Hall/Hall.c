#include "Hall.h"
/**
 * @brief ��TIM2��ʼ��Ϊ�������ӿ�ģʽ
 * @param psc Ԥ��Ƶϵ��
 * @param arr �Զ���װ��ֵ
 * @retval None
 */
void Encoder_Init_TIM2(uint16_t psc,uint16_t arr)	
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    //NVIC_InitTypeDef  NVIC_InitStructure;



    //ʹ�ܶ�ʱ��2��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //ʹ��PB�˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);



    //�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    //��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    //�����趨������ʼ��GPIOB
    GPIO_Init(GPIOA, &GPIO_InitStructure);



    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    // Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    //�趨�������Զ���װֵ
    TIM_TimeBaseStructure.TIM_Period = arr;
    //ѡ��ʱ�ӷ�Ƶ������Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //TIM���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);



    //ʹ�ñ�����ģʽ3
    TIM_EncoderInterfaceConfig(TIM2, \
                               TIM_EncoderMode_TI12, \
                               TIM_ICPolarity_Rising, \
                               TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);



    //���TIM�ĸ��±�־λ
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    //Reset counter
    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief ��λʱ���ȡ����������
 * @param TIMX ��ʱ��
 * @retval �ٶ�ֵ	�Ǳ��������ص�����
 */
int Read_Encoder()
{
    	int Encoder_TIM;
        Encoder_TIM= (short)TIM2 -> CNT;
        //Encoder_TIM= (int)((int16_t)(TIM4->CNT));;
        TIM2 -> CNT=0;
   	    return Encoder_TIM;
}
