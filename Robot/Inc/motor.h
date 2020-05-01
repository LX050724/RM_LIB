/**
 * @file    motor.h
 * @author  yao
 * @date    1-May-2020
 * @brief   电机驱动模块头文件
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

#if defined(STM32F407xx) || defined(STM32F405xx) || defined(STM32F427xx)
#include <stm32f4xx.h>
#elif defined(STM32F303xx) || defined(STM32F334xx)
#include <stm32f3xx.h>
#elif defined(STM32F103xx)
#include <stm32f1xx.h>
#endif

#include "CANDrive.h"

#define RM3508_LIMIT 16384  //!<@brief RM3508的输出限幅
#define GM6020_LIMIT 30000  //!<@brief GM6020的输出限幅
#define RM3510_LIMIT 32767  //!<@brief RM3510的输出限幅
#define GM3510_LIMIT 29000  //!<@brief GM3510的输出限幅
#define M2006_LIMIT  10000  //!<@brief M2006 的输出限幅
#define RM6623_LIMIT 32767  //!<@brief RM6623的输出限幅(找不到了)

/**
 * @brief RM3508电机数据结构体
 */
typedef struct __RM3508_TypeDef {
    uint16_t MchanicalAngle;    //!<@brief 机械角度
    int16_t Speed;              //!<@brief 转速
    int16_t TorqueCurrent;      //!<@brief 转矩电流
    uint8_t temp;               //!<@brief 温度
    float Power;                //!<@brief 功率
    uint16_t LsatAngle;         //!<@brief 上一次的机械角度
    int16_t r;                  //!<@brief 圈数
    int32_t Angle;              //!<@brief 连续化机械角度
    float Angle_DEG;            //!<@brief 连续化角度制角度
    struct PowerCOF_s {
        float ss;               //!<@brief 速度平方项系数
        float sc;               //!<@brief 速度,转矩电流乘积项系数
        float cc;               //!<@brief 转矩电流平方项系数
        float constant;         //!<@brief 常量
    } PowerCOF;                  //!<@brief 计算功率所用的系数,由MATLAB拟合
} RM3508_TypeDef;

/**
 * @brief GM6020电机数据结构体
 */
typedef struct __GM6020_TypeDef {
    uint16_t MchanicalAngle;    //!<@brief 机械角度
    int16_t Speed;              //!<@brief 转速
    int16_t TorqueCurrent;      //!<@brief 转矩电流
    uint8_t temp;               //!<@brief 温度
    uint16_t LsatAngle;         //!<@brief 上一次的机械角度
    int16_t r;                  //!<@brief 圈数
    int32_t Angle;              //!<@brief 连续化机械角度
    float Angle_DEG;            //!<@brief 连续化角度制角度
} GM6020_TypeDef;

/**
 * @brief RM3510电机数据结构体
 */
typedef struct __RM3510_TypeDef {
    uint16_t MchanicalAngle;    //!<@brief 机械角度    
    int16_t Speed;              //!<@brief 转速
} RM3510_TypeDef;

/**
 * @brief GM3510电机数据结构体
 */
typedef struct __GM3510_TypeDef {
    uint16_t MchanicalAngle;    //!<@brief 机械角度
    int16_t OutputTorque;       //!<@brief 输出扭矩
    uint16_t LsatAngle;         //!<@brief 上一次的机械角度
    int16_t r;                  //!<@brief 圈数
    int32_t Angle;              //!<@brief 连续化机械角度
    float Angle_DEG;            //!<@brief 连续化角度制角度
} GM3510_TypeDef;

/**
 * @brief M2006电机数据结构体
 */
typedef struct __M2006_TypeDef {
    uint16_t MchanicalAngle;    //!<@brief 机械角度
    int16_t Speed;              //!<@brief 转速
    uint16_t LsatAngle;         //!<@brief 上一次的机械角度
    int16_t r;                  //!<@brief 圈数
    int32_t Angle;              //!<@brief 连续化机械角度
    float Angle_DEG;            //!<@brief 连续化角度制角度
} M2006_TypeDef;

/**
 * @brief RM6623电机数据结构体
 */
typedef struct __RM6623_TypeDef {
    uint16_t MchanicalAngle;    //!<@brief 机械角度
    int16_t TorqueCurrent;      //!<@brief 转矩电流
    int16_t SetTorqueCurrent;   //!<@brief 设定转矩电流
    uint16_t LsatAngle;         //!<@brief 上一次的机械角度
    int16_t r;                  //!<@brief 圈数
    int32_t Angle;              //!<@brief 连续化机械角度
    float Angle_DEG;            //!<@brief 连续化角度制角度
} RM6623_TypeDef;


void RM6623_Receive(RM6623_TypeDef *Dst, uint8_t *Data);

void RM3510_Receive(RM3510_TypeDef *Dst, uint8_t *Data);

void RM3508_SetPowerCOF(RM3508_TypeDef *Dst, float cc, float sc, float ss, float constant);

void RM3508_Receive(RM3508_TypeDef *Dst, uint8_t *Data);

void GM6020_Receive(GM6020_TypeDef *Dst, uint8_t *Data);

void M2006_Receive(M2006_TypeDef *Dst, uint8_t *Data);

void GM3510_Receive(GM3510_TypeDef *Dst, uint8_t *Data);

HAL_StatusTypeDef MotorSend(can_num_e can, uint32_t STD_ID, int16_t *Data);

#endif
