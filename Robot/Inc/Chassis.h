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
typedef struct __ChassisSpeed_Ref_t
{
     int16_t forward_back_ref;
     int16_t left_right_ref;
     int16_t rotate_ref;
} ChassisSpeed_Ref_t;

/**
 * @brief 云台角度结构体
 */
typedef struct __PTZAngle_Ref_t
{
     float Pitch;
     float Yaw;
} PTZAngle_Ref_t;

/**
 * @brief 轮组速度结构体
 */
typedef struct __Chassis_Motor_PID_Expect
{
     int16_t Chassis_Motor_PID_Expect_1;
     int16_t Chassis_Motor_PID_Expect_2;
     int16_t Chassis_Motor_PID_Expect_3;
     int16_t Chassis_Motor_PID_Expect_4;
} Chassis_Motor_PID_Expect;

void Chassis_Speed_Ref_Zero(ChassisSpeed_Ref_t* ref);//速度清零
__weak void PID_Expect(Chassis_Motor_PID_Expect* motor,ChassisSpeed_Ref_t* ref); //得到底盘电机所期望的速度

#endif
