/**
 * @file    WatchDog.h
 * @author  yao
 * @date    1-May-2020
 * @brief   软件看门狗模块头文件
 */

#ifndef _WATCH_DOG_H_
#define _WATCH_DOG_H_

#if defined(WatchDoglength) && WatchDoglength > 0

#include "RMLibHead.h"

RMLIB_CPP_BEGIN

/**
 * @brief 看门狗结构体
 */
typedef struct {
    uint32_t Life;  //!<@brief 当前离线计数
    uint32_t Max;   //!<@brief 最大离线计数
} WatchDog_TypeDef, *WatchDogp;

/**
 * @brief 看门狗轮询函数，需要以一定周期调用
 * @note 在无系统时可以使用重写HAL_IncTick函数的方式轮询  
 *       例：  
 *       {@code 
 *       void HAL_IncTick(void) {
 *           uwTick += uwTickFreq;
 *           WatchDog_Polling();
 *       }}
 *       在实时系统下则需要创建一个任务周期性调用该函数
 */
void WatchDog_Polling(void);

/**
 * @brief 判断是否为同一个看门狗
 * @param handle 看门狗结构体指针
 * @param Dog 要判断的看门狗
 */
#define IS_Dog(handle, Dog) ((handle) == &(Dog))

/**
 * @brief 初始化看门狗
 * @param[out] handle 看门狗结构体指针
 * @param[in] Life
 */
void WatchDog_Init(WatchDogp handle, uint32_t Life);


/**
 * @brief 看门狗回调
 * @param[in] handle 看门狗结构体指针
 */
void WatchDog_CallBack(WatchDogp handle);

/**
 * @brief 喂狗事件回调
 * @param[in] handle 看门狗结构体指针
 */
void FeedDog_CallBack(WatchDogp handle);

/**
 * @brief 喂狗函数
 * @param[in] handle 看门狗结构体指针
 */
void Feed_Dog(WatchDogp handle);

RMLIB_CPP_END

#endif

#endif
