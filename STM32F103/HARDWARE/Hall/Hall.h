#ifndef __Hall_H
#define __Hall_H


#include "sys.h"
	 
void Encoder_Init_TIM2(uint16_t psc,uint16_t arr);
int Read_Encoder(void);


#endif
