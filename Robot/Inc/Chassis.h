/**
 * @file    Chassis.h
 * @author  yao
 * @date    1-May-2020
 * @brief   底盘驱动模块头文件
 */

#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#if defined(STM32F407xx) || defined(STM32F405xx) || defined(STM32F427xx)
#include <stm32f4xx.h>
#elif defined(STM32F303xx) || defined(STM32F334xx)
#include <stm32f3xx.h>
#elif defined(STM32F103xx)
#include <stm32f1xx.h>
#endif

/**
 * @brief 矢量速度结构体
 */
typedef struct __ChassisSpeed_Ref_t {
    int16_t forward_back_ref;  //!<@brief 前进方向速度
    int16_t left_right_ref;    //!<@brief 左右方向速度
    int16_t rotate_ref;        //!<@brief 旋转速度
} ChassisSpeed_Ref_t;

/**
 * @brief 云台角度结构体
 */
typedef struct __PTZAngle_Ref_t {
    float Pitch;               //!<@brief Pitch轴角度
    float Yaw;                 //!<@brief Yaw轴角度
} PTZAngle_Ref_t;

/**
 * @brief 轮组速度结构体
 */
typedef struct __Chassis_Motor_Speed {
    int16_t speed_1;    //!<@brief 底盘电机1速度
    int16_t speed_2;    //!<@brief 底盘电机2速度
    int16_t speed_3;    //!<@brief 底盘电机3速度
    int16_t speed_4;    //!<@brief 底盘电机4速度
} Chassis_Motor_Speed;

void ChassisMotorSpeedClean(ChassisSpeed_Ref_t *ref);

__weak void PID_Expect(Chassis_Motor_Speed *motor, ChassisSpeed_Ref_t *ref);

#endif
