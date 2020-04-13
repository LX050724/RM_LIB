#include "ramp.h"

__weak uint32_t Get_TimerTick()//使用前必须重写该函数，(0.1MHz),推荐使用inline修饰内联函数
{
    return 0;
}

inline float Slope(Ramp_Typedef* Ramp) //斜坡函数，让缓慢增速
{
    if(!Ramp->flag)
    {
        Ramp->StartTick = Get_TimerTick();
        Ramp->flag = 1;
    }
    if(Get_TimerTick() > (Ramp->StartTick + Ramp->RampTime))return 1.0f;
    return ((Get_TimerTick() - Ramp->StartTick) / (float)Ramp->RampTime);
}

inline void ResetSlope(Ramp_Typedef* Ramp) //释放只是为了让下次在进斜坡
{
    Ramp->flag = 0;
}
