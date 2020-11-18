/**
 * @file    CANDrive.h
 * @author  yao
 * @date    1-May-2020
 * @brief   CAN底层驱动模块头文件
 * @note
 *  针对cube生成的代码编写！
 *  如果使用实时系统添加全局宏定义__USE_RTOS启用发送临界区保护
 */

#ifndef _CANDrive_H_
#define _CANDrive_H_

#include "RMLibHead.h"
#ifdef HAL_CAN_MODULE_ENABLED
#include "can.h"

/**
 * @brief can枚举
 */
typedef enum
{
    can1 = 1,   //!<@brief CAN1标志
    can2 = 2,   //!<@brief CAN2标志
}can_num_e;

extern CAN_RxHeaderTypeDef CAN1_Rx; //!<@brief CAN1接收句柄
extern CAN_TxHeaderTypeDef CAN1_Tx; //!<@brief CAN1发送句柄
extern uint8_t CAN1_buff[8];        //!<@brief CAN1接收缓冲区

/**
 * @brief  按照通常设置初始化CAN滤波器
 * @param[in] hcan CAN handle Structure definition
 */
void CanFilter_Init(CAN_HandleTypeDef* hcan);

/**
 * @brief CAN1发送标准帧数据
 * @param[in] StdId 标准帧ID
 * @param[in] msg 数据数组,长度为8
 * @return HAL Status structures definition
 */
HAL_StatusTypeDef CAN1_Send_Msg(uint32_t StdId, uint8_t *msg);

/**
 * @brief CAN1读取数据
 * @param[out] buf 数据缓冲区
 * @return HAL Status structures definition
 */
static inline HAL_StatusTypeDef CAN1_Receive_Msg(uint8_t *buf) {
    return HAL_CAN_GetRxMessage(&hcan1, CAN_FilterFIFO0, &CAN1_Rx, buf);
}

#ifdef CAN2_SUPPORT
extern CAN_RxHeaderTypeDef CAN2_Rx;  //!<@brief CAN2接收句柄
extern CAN_TxHeaderTypeDef CAN2_Tx;  //!<@brief CAN2发送句柄
extern uint8_t CAN2_buff[8];         //!<@brief CAN2接收缓冲区

/**
 * @brief CAN2发送标准帧数据
 * @param[in] StdId 标准帧ID
 * @param[in] msg 数据数组,长度为8
 * @return HAL Status structures definition
 */
HAL_StatusTypeDef CAN2_Send_Msg(uint32_t StdId, uint8_t *msg);

/**
 * @brief CAN2读取数据
 * @param[out] buf 数据缓冲区
 * @return HAL Status structures definition
 */
inline HAL_StatusTypeDef CAN2_Receive_Msg(uint8_t *buf){
    return HAL_CAN_GetRxMessage(&hcan2, CAN_FilterFIFO1, &CAN2_Rx, buf);
}

#endif //CAN2_SUPPORT

#endif //HAL_CAN_MODULE_ENABLED

#endif //_CANDrive_H_
