#ifndef _WATCH_DOG_H_
#define _WATCH_DOG_H_

#if defined(WatchDoglength) && WatchDoglength > 0

#if defined(STM32F407xx) || defined(STM32F405xx) || defined(STM32F427xx)

#include <stm32f4xx.h>

#elif defined(STM32F303xx) || defined(STM32F334xx)
#include <stm32f3xx.h>
#elif defined(STM32F103xx)
#include <stm32f1xx.h>
#endif

/**
 * @brief 看门狗结构体
 */
typedef struct __WatchDog {
    uint32_t Life;
    uint32_t Max;
} WatchDog_TypeDef, *WatchDogp;

/**
 * @brief 判断是否为同一个看门狗
 * @param handle 看门狗结构体指针
 * @param Dog 要判断的看门狗
 */
#define IS_Dog(handle, Dog) ((handle) == &(Dog))

void WatchDog_CallBack(WatchDogp handle);

void FeedDog_CallBack(WatchDogp handle);

void Feed_Dog(WatchDogp handle);

void WatchDog_Init(WatchDogp handle, uint32_t Life);

#endif
#endif
