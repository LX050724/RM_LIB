#ifndef _CANDrive_H_
#define _CANDrive_H_

#if defined(STM32F407xx) || defined(STM32F405xx) || defined(STM32F427xx)
	#include <stm32f4xx.h>
#elif defined(STM32F303xx) || defined(STM32F334xx)
	#include <stm32f3xx.h>
#elif defined(STM32F103xx)
	#include <stm32f1xx.h>
#endif

#ifdef __USE_RTOS
	#include "FreeRTOS.h"
	#include "cmsis_os.h"
	#define CAN_ENTER_CRITICAL() taskENTER_CRITICAL()
	#define CAN_EXIT_CRITICAL() taskEXIT_CRITICAL()
#else
	#define CAN_ENTER_CRITICAL()
	#define CAN_EXIT_CRITICAL()
#endif 

typedef enum
{
	can1 = 1,
	can2 = 2,
}can_num_e;

extern CAN_RxHeaderTypeDef CAN1_Rx;
extern CAN_TxHeaderTypeDef CAN1_Tx;
extern uint8_t CAN1_buff[8];

void CanFilter_Init(CAN_HandleTypeDef* hcan);
HAL_StatusTypeDef CAN1_Send_Msg(uint32_t StdId, uint8_t *msg);
HAL_StatusTypeDef CAN1_Receive_Msg(uint8_t *buf);

#ifdef CAN2_SUPPORT
extern CAN_RxHeaderTypeDef CAN2_Rx;
extern CAN_TxHeaderTypeDef CAN2_Tx;
extern uint8_t CAN2_buff[8];
HAL_StatusTypeDef CAN2_Send_Msg(uint32_t StdId, uint8_t *msg);
HAL_StatusTypeDef CAN2_Receive_Msg(uint8_t *buf);
#endif

#endif
