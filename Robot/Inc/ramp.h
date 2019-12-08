#ifndef __ramp_H
#define __ramp_H

#include "stm32f4xx_hal.h"
#include "tim.h"

typedef struct{
	uint8_t flag;
	uint32_t StartTick;
	uint32_t RampTime;
}Ramp_Typedef;

uint32_t Get_TimerTick();//使用前需重写该函数，功能为得到时钟计数
float Slope(Ramp_Typedef* Ramp);//斜坡函数
void ResetSlope(Ramp_Typedef* Ramp);//斜坡复位

#endif /*__ usart_H */
