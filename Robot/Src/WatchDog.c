/**
 * @file    WatchDog.c
 * @author  yao
 * @date    1-May-2020
 * @brief   看门狗模块
 * @details 要启用看门狗需要添加全局宏定义WatchDoglength并赋值
 *  需要的最大看门狗数量
 */

#include <WatchDog.h>

#if defined(WatchDoglength) && WatchDoglength > 0

extern volatile uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;

/*!@brief 看门狗列表*/
static WatchDogp List[WatchDoglength];

/*!@brief 看门狗长度*/
static uint16_t Len = 0;

/**
 * @brief 重写HAL_IncTick函数实现看门狗轮询
 */
void HAL_IncTick(void) {
    uwTick += uwTickFreq;
    for (uint8_t i = 0; i < Len; ++i) {
        List[i]->Life++;
        if (List[i]->Life > List[i]->Max) {
            WatchDog_CallBack(List[i]);
        }
    }
}

/**
 * @brief 初始化看门狗
 * @param[out] handle 看门狗结构体指针
 * @param[in] Life 
 */
void WatchDog_Init(WatchDogp handle, uint32_t Life) {
    if (Len >= WatchDoglength)
        return;
    handle->Max = Life;
    List[Len++] = handle;
}

/**
 * @brief 喂狗函数
 * @param[in] handle 看门狗结构体指针
 */
void Feed_Dog(WatchDogp handle) {
    handle->Life = 0;
    FeedDog_CallBack(handle);
}

/**
 * @brief 喂狗事件回调
 * @param[in] handle 看门狗结构体指针
 */
__weak void FeedDog_CallBack(WatchDogp handle) {
    UNUSED(handle);
}

/**
 * @brief 看门狗回调
 * @param[in] handle 看门狗结构体指针
 */
__weak void WatchDog_CallBack(WatchDogp handle) {
    UNUSED(handle);
}

#endif
