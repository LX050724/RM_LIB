#include "ramp.h"

__weak uint32_t Get_TimerTick() {
    return HAL_GetTick();
}

float Slope(Ramp_Typedef *Ramp) {
    if (!Ramp->flag) {
        Ramp->StartTick = Get_TimerTick();
        Ramp->flag = 1;
    }
    if (Get_TimerTick() > (Ramp->StartTick + Ramp->RampTime))return 1.0f;
    return ((Get_TimerTick() - Ramp->StartTick) / (float) Ramp->RampTime);
}
