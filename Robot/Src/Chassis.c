/**
 * @file    Chassis.c
 * @author  yao
 * @date    1-May-2020
 * @brief   底盘驱动模块
 */

#include "Chassis.h"

/**
 * @brief 将预期的速度矢量清零
 * @param[out] ref 矢量速度结构体
 */
void ChassisMotorSpeedClean(ChassisSpeed_Ref_t *ref) {
    ref->forward_back_ref = 0;
    ref->left_right_ref = 0;
    ref->rotate_ref = 0;
}

/**
 * @brief 由速度矢量计算电机速度，该函数是弱函数，可以重新以适应不同的底盘
 * @param[out] motor 轮组速度结构体
 * @param[in] ref 矢量速度结构体
 */
__weak void PID_Expect(Chassis_Motor_Speed *motor, ChassisSpeed_Ref_t *ref) {
    motor->speed_3 = -ref->forward_back_ref -
                     ref->left_right_ref + ref->rotate_ref;

    motor->speed_2 = ref->forward_back_ref -
                     ref->left_right_ref + ref->rotate_ref;

    motor->speed_1 = ref->forward_back_ref +
                     ref->left_right_ref + ref->rotate_ref;

    motor->speed_4 = -ref->forward_back_ref +
                     ref->left_right_ref + ref->rotate_ref;
}
