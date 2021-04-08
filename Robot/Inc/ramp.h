/**
 * @file    ramp.h
 * @author  yao
 * @date    1-May-2020
 * @brief   斜坡函数模块
 * @details 定义Ramp_Typedef时应初始化斜坡时间RampTime为指定的时间
 *
 * Ramp_Typedef ramp = {.RampTime = 70000};
 */

#ifndef __ramp_H
#define __ramp_H

#include "RMLibHead.h"

RMLIB_CPP_BEGIN

/**
 * @brief 斜坡函数结构体
 * @note  定义时应初始化斜坡时间RampTime为指定的时间
 */
typedef struct {
    uint8_t flag;       //!<@brief 启动标志
    uint32_t StartTick; //!<@brief 起始时间
    uint32_t RampTime;  //!<@brief 斜坡时间
} Ramp_Typedef;

/**
 * @brief 斜坡函数,弱函数可重写为自定义时钟源,默认SysTick
 * @note 可重写该函数，更换为自定义时钟源
 * @return 时钟计数值
 */
uint32_t Get_TimerTick(void);

/**
 * @brief 斜坡函数
 * @param[in] Ramp 斜坡函数结构体指针
 * @return 0-1的随时间匀速上升的变量
 */
float Slope(Ramp_Typedef *Ramp);

/**
 * @brief 重置斜坡
 * @param[in] Ramp 斜坡函数结构体指针
 */
inline void ResetSlope(Ramp_Typedef *Ramp) {
    Ramp->flag = 0;
}

RMLIB_CPP_END

#endif
