#include <WatchDog.h>

#if defined(WatchDoglength) && WatchDoglength > 0

extern volatile uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;

WatchDogp List[WatchDoglength];
uint16_t Len = 0;

void HAL_IncTick(void) {
    uwTick += uwTickFreq;
    for (uint8_t i = 0; i < Len; ++i) {
        List[i]->Life++;
        if (List[i]->Life > List[i]->Max) {
            WatchDog_CallBack(List[i]);
        }
    }
}

void WatchDog_Init(WatchDogp handle, uint32_t Life) {
    if (Len >= WatchDoglength)
        return;
    handle->Max = Life;
    List[Len++] = handle;
}

/**
 * @brief 喂狗函数
 * @param handle 看门狗结构体指针
 */
void Feed_Dog(WatchDogp handle) {
    handle->Life = 0;
    FeedDog_CallBack(handle);
}

/**
 * @brief 喂狗事件回调
 * @param handle 看门狗结构体指针
 */
__weak void FeedDog_CallBack(WatchDogp handle) {
    UNUSED(handle);
}

/**
 * @brief 看门狗回调
 * @param handle 看门狗结构体指针
 */
__weak void WatchDog_CallBack(WatchDogp handle) {
    UNUSED(handle);
}

#endif
