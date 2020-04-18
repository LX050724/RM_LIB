#include "ramp.h"

/**
 * @brief 斜坡函数,可重写为自定义时钟源,默认SysTick
 * @return 时钟计数值
 */
__weak uint32_t Get_TimerTick() {
    return HAL_GetTick();
}

/**
 * @brief 斜坡函数
 * @param Ramp 斜坡函数结构体指针
 * @return 0-1的随时间匀速上升的变量
 */
float Slope(Ramp_Typedef *Ramp) {
    if (!Ramp->flag) {
        Ramp->StartTick = Get_TimerTick();
        Ramp->flag = 1;
    }
    if (Get_TimerTick() > (Ramp->StartTick + Ramp->RampTime))return 1.0f;
    return ((Get_TimerTick() - Ramp->StartTick) / (float) Ramp->RampTime);
}

/**
 * @brief 重置斜坡
 * @param Ramp 斜坡函数结构体指针
 */
void ResetSlope(Ramp_Typedef *Ramp) {
    Ramp->flag = 0;
}
