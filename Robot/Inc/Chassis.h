#ifndef _CHASSIS_H_
#define _CHASSIS_H_
#include "stm32f4xx_hal.h"

typedef struct
{
     int16_t forward_back_ref;
     int16_t left_right_ref;
     int16_t rotate_ref;
} ChassisSpeed_Ref_t;

typedef struct
{
     float Pitch;
     float Yaw;
} PTZAngle_Ref_t;

typedef struct
{
     int16_t Chassis_Motor_PID_Expect_1;
     int16_t Chassis_Motor_PID_Expect_2;
     int16_t Chassis_Motor_PID_Expect_3;
     int16_t Chassis_Motor_PID_Expect_4;
} Chassis_Motor_PID_Expect;

void Chassis_Speed_Ref_Zero(ChassisSpeed_Ref_t* ref);
void PID_Expect(Chassis_Motor_PID_Expect* motor,ChassisSpeed_Ref_t* ref); //得到底盘电机所期望的速度

#endif
