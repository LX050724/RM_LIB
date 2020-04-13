#ifndef __ramp_H
#define __ramp_H

#if defined(STM32F407xx) || defined(STM32F405xx) || defined(STM32F427xx)
    #include <stm32f4xx.h>
#elif defined(STM32F303xx) || defined(STM32F334xx)
    #include <stm32f3xx.h>
#elif defined(STM32F103xx)
    #include <stm32f1xx.h>
#endif
typedef struct{
    uint8_t flag;
    uint32_t StartTick;
    uint32_t RampTime;
}Ramp_Typedef;

uint32_t Get_TimerTick(void);//使用前需重写该函数，功能为得到时钟计数
float Slope(Ramp_Typedef* Ramp);//斜坡函数
void ResetSlope(Ramp_Typedef* Ramp);//斜坡复位

#endif
