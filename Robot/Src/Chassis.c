/**
 * @file    Chassis.c
 * @author  yao
 * @date    1-May-2020
 * @brief   底盘驱动模块
 */

#include "Chassis.h"


void ChassisMotorSpeed_clean(ChassisSpeed_Ref_t *ref) {
    ref->forward_back_ref = 0;
    ref->left_right_ref = 0;
    ref->rotate_ref = 0;
}

__weak void ChassisMotorSpeed_get(Chassis_Motor_Speed *motor, ChassisSpeed_Ref_t *ref) {
    motor->speed_3 = -ref->forward_back_ref -
                     ref->left_right_ref + ref->rotate_ref;

    motor->speed_2 = ref->forward_back_ref -
                     ref->left_right_ref + ref->rotate_ref;

    motor->speed_1 = ref->forward_back_ref +
                     ref->left_right_ref + ref->rotate_ref;

    motor->speed_4 = -ref->forward_back_ref +
                     ref->left_right_ref + ref->rotate_ref;
}
