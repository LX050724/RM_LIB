/**
 * @file    ramp.h
 * @author  yao
 * @date    1-May-2020
 * @brief   斜坡函数模块头文件
 */

#ifndef __ramp_H
#define __ramp_H

#if defined(STM32F407xx) || defined(STM32F405xx) || defined(STM32F427xx)
#include <stm32f4xx.h>
#elif defined(STM32F303xx) || defined(STM32F334xx)
#include <stm32f3xx.h>
#elif defined(STM32F103xx)
#include <stm32f1xx.h>
#endif

/**
 * @brief 斜坡函数结构体
 * @note  定义时应初始化斜坡时间RampTime为指定的时间
 */
typedef struct __Ramp_Typedef {
    uint8_t flag;       //!<@brief 启动标志
    uint32_t StartTick; //!<@brief 起始时间
    uint32_t RampTime;  //!<@brief 斜坡时间
} Ramp_Typedef;

uint32_t Get_TimerTick(void);

float Slope(Ramp_Typedef *Ramp);

void ResetSlope(Ramp_Typedef *Ramp);

#endif
