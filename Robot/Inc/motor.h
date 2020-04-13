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

#define RM3508_LIMIT 16384
#define GM6020_LIMIT 30000
#define RM3510_LIMIT 32767
#define GM3510_LIMIT 29000
#define M2006_LIMIT  10000
#define RM6623_LIMIT 32767 //找不到了

typedef struct
{
    uint16_t MchanicalAngle;
    int16_t Speed;
    int16_t TorqueCurrent;
    uint8_t temp;
    float Power;
    uint16_t LsatAngle;
    int16_t r;
    int32_t Angle;
    float Angle_DEG;
    struct PowerCOF_s{
        float ss;
        float sc;
        float cc;
    }PowerCOF;
}RM3508_TypeDef;

typedef struct
{
    uint16_t MchanicalAngle;
    int16_t Speed;
    int16_t TorqueCurrent;
    uint8_t temp;
    uint16_t LsatAngle;
    int16_t r;
    int32_t Angle;
    float Angle_DEG;
}GM6020_TypeDef;

typedef struct
{
    uint16_t MchanicalAngle;
    int16_t Speed;
}RM3510_TypeDef;

typedef struct
{
    uint16_t MchanicalAngle;
    int16_t OutputTorque;
    uint16_t LsatAngle;
    int16_t r;
    int32_t Angle;
    float Angle_DEG;
}GM3510_TypeDef;

typedef struct
{
    uint16_t MchanicalAngle;
    int16_t Speed;
    uint16_t LsatAngle;
    int16_t r;
    int32_t Angle;
    float Angle_DEG;
}M2006_TypeDef;

typedef struct
{
    uint16_t MchanicalAngle;
    int16_t TorqueCurrent;
    int16_t SetTorqueCurrent;
    uint16_t LsatAngle;
    int16_t r;
    int32_t Angle;
    float Angle_DEG;
}RM6623_TypeDef;


void RM6623_Receive(RM6623_TypeDef* Dst,uint8_t* Data);
void RM3510_Receive(RM3510_TypeDef* Dst,uint8_t* Data);
void RM3508_SetPowerCOF(RM3508_TypeDef *Dst, float cc, float sc, float ss);
void RM3508_Receive(RM3508_TypeDef* Dst,uint8_t* Data);
void GM6020_Receive(GM6020_TypeDef* Dst,uint8_t* Data);
void M2006_Receive(M2006_TypeDef* Dst,uint8_t* Data);
void GM3510_Receive(GM3510_TypeDef* Dst,uint8_t* Data);
HAL_StatusTypeDef MotorSend(can_num_e can,uint32_t STD_ID,int16_t* Data);

#endif
