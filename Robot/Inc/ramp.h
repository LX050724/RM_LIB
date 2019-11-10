#ifndef __ramp_H
#define __ramp_H

#include "stm32f4xx_hal.h"
#include "tim.h"

typedef struct{
	uint8_t flag;
	uint32_t StartTick;
	uint32_t RampTime;
}Ramp_Typedef;

uint32_t Get_TimerTick();
float Slope(Ramp_Typedef* Ramp);
void ResetSlope(Ramp_Typedef* Ramp);

#endif /*__ usart_H */
