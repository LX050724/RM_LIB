#ifndef _TEST__CAN_H
#define _TEST__CAN_H
#include "stm32f4xx_hal.h"
#ifdef HAL_CAN_MODULE_ENABLED

#define CAN2_ENABLE 1

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

#if CAN2_ENABLE == 1
extern CAN_RxHeaderTypeDef CAN2_Rx;
extern CAN_TxHeaderTypeDef CAN2_Tx;
extern uint8_t CAN2_buff[8];
HAL_StatusTypeDef CAN2_Send_Msg(uint32_t StdId, uint8_t *msg);
HAL_StatusTypeDef CAN2_Receive_Msg(uint8_t *buf);
#endif

#endif
#endif
