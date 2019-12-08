#ifndef _MOTOR_H_
#define _MOTOR_H_
#include <stm32f4xx_hal.h>

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
void RM3508_Receive(RM3508_TypeDef* Dst,uint8_t* Data);
void GM6020_Receive(GM6020_TypeDef* Dst,uint8_t* Data);
void M2006_Receive(M2006_TypeDef* Dst,uint8_t* Data);
HAL_StatusTypeDef MotorSend(uint8_t can,uint32_t STD_ID,int16_t* Data);

#endif
