/**
 * @file    Chassis.h
 * @author  yao
 * @date    1-May-2020
 * @brief   底盘驱动模块头文件
 */

#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#include "RMLibHead.h"

RMLIB_CPP_BEGIN

/**
 * @brief 矢量速度结构体
 */
typedef struct {
    int16_t forward_back_ref;  //!<@brief 前进方向速度
    int16_t left_right_ref;    //!<@brief 左右方向速度
    int16_t rotate_ref;        //!<@brief 旋转速度
} ChassisSpeed_Ref_t;

/**
 * @brief 云台角度结构体
 */
typedef struct {
    float Pitch;               //!<@brief Pitch轴角度
    float Yaw;                 //!<@brief Yaw轴角度
} PTZAngle_Ref_t;

/**
 * @brief 轮组速度结构体
 */
typedef struct {
    int16_t speed_1;    //!<@brief 底盘电机1速度
    int16_t speed_2;    //!<@brief 底盘电机2速度
    int16_t speed_3;    //!<@brief 底盘电机3速度
    int16_t speed_4;    //!<@brief 底盘电机4速度
} Chassis_Motor_Speed;

/**
 * @brief 将预期的速度矢量清零
 * @param[out] ref 矢量速度结构体
 */
void ChassisMotorSpeed_clean(ChassisSpeed_Ref_t *ref);

/**
 * @brief 由速度矢量计算电机速度，该函数是弱函数，可以重写以适应不同的底盘
 * @param[out] motor 轮组速度结构体
 * @param[in] ref 矢量速度结构体
 */
void ChassisMotorSpeed_get(Chassis_Motor_Speed *motor, ChassisSpeed_Ref_t *ref);

RMLIB_CPP_END

#endif
