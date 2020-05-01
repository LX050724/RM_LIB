/**
 * @file    CANDrive.c
 * @author  yao
 * @date    1-May-2020
 * @brief   CAN底层驱动模块
 * @note    针对cube生成的代码编写！
 */

#include "CANDrive.h"
#include "can.h"

#ifdef HAL_CAN_MODULE_ENABLED
CAN_RxHeaderTypeDef CAN1_Rx;    //!<@brief CAN1接收句柄
CAN_TxHeaderTypeDef CAN1_Tx;    //!<@brief CAN1发送句柄
uint8_t CAN1_buff[8];           //!<@brief CAN1接收缓冲区

#ifdef CAN2_SUPPORT
CAN_RxHeaderTypeDef CAN2_Rx;    //!<@brief CAN2接收句柄
CAN_TxHeaderTypeDef CAN2_Tx;    //!<@brief CAN2发送句柄
uint8_t CAN2_buff[8];           //!<@brief CAN2接收缓冲区
#endif

/**
 * @brief  按照通常设置初始化CAN滤波器
 * @param[in] hcan CAN handle Structure definition
 */
void CanFilter_Init(CAN_HandleTypeDef *hcan) {
    CAN_FilterTypeDef canfilter;

    canfilter.FilterMode = CAN_FILTERMODE_IDMASK;
    canfilter.FilterScale = CAN_FILTERSCALE_32BIT;

    canfilter.FilterIdHigh = 0x0000;
    canfilter.FilterIdLow = 0x0000;
    canfilter.FilterMaskIdHigh = 0x0000;
    canfilter.FilterMaskIdLow = 0x0000;

    /* 从can的过滤器起始编号 只有当设置两个can时 该参数才有意义 */
    canfilter.SlaveStartFilterBank = 14;

    /*can1和CAN2使用不同的滤波器*/
    if (hcan->Instance == CAN1) {
        /* 主can的过滤器编号 */
        canfilter.FilterBank = 0;

        /* CAN_FilterFIFO0 */
        canfilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    }
#ifdef CAN2_SUPPORT
    if (hcan->Instance == CAN2) {
        /* 从can的过滤器编号 */
        canfilter.FilterBank = 14;

        /* CAN_FilterFIFO1 */
        canfilter.FilterFIFOAssignment = CAN_FILTER_FIFO1;  // ;
    }
#endif

    /* 激活过滤器 */
    canfilter.FilterActivation = ENABLE;
    HAL_CAN_ConfigFilter(hcan, &canfilter);
}

/**
 * @brief CAN1发送标准帧数据
 * @param[in] StdId 标准帧ID
 * @param[in] msg 数据数组,长度为8
 * @return HAL Status structures definition
 */
HAL_StatusTypeDef CAN1_Send_Msg(uint32_t StdId, uint8_t *msg) {
    CAN_ENTER_CRITICAL();

    /* 标准标识符 */
    CAN1_Tx.StdId = StdId;

    /* 使用标准帧 */
    CAN1_Tx.IDE = CAN_ID_STD;

    /* 数据帧 */
    CAN1_Tx.RTR = CAN_RTR_DATA;

    CAN1_Tx.DLC = 8;
    CAN1_Tx.TransmitGlobalTime = DISABLE;

    HAL_StatusTypeDef err = HAL_CAN_AddTxMessage(&hcan1, &CAN1_Tx, msg, (uint32_t *) CAN_TX_MAILBOX0);
    CAN_EXIT_CRITICAL();
    return err;
}

/**
 * @brief CAN1读取数据
 * @param[out] buf 数据缓冲区
 * @return HAL Status structures definition
 */
HAL_StatusTypeDef CAN1_Receive_Msg(uint8_t *buf) {
    return HAL_CAN_GetRxMessage(&hcan1, CAN_FilterFIFO0, &CAN1_Rx, buf);
}

#ifdef CAN2_SUPPORT

/**
 * @brief CAN2发送标准帧数据
 * @param[in] StdId 标准帧ID
 * @param[in] msg 数据数组,长度为8
 * @return HAL Status structures definition
 */
HAL_StatusTypeDef CAN2_Send_Msg(uint32_t StdId, uint8_t *msg) {
    CAN_ENTER_CRITICAL();

    /* 标准标识符 */
    CAN2_Tx.StdId = StdId;

    /* 使用标准帧 */
    CAN2_Tx.IDE = CAN_ID_STD;

    /* 数据帧 */
    CAN2_Tx.RTR = CAN_RTR_DATA;

    CAN2_Tx.DLC = 8;
    CAN2_Tx.TransmitGlobalTime = DISABLE;
    HAL_StatusTypeDef err = HAL_CAN_AddTxMessage(&hcan2, &CAN2_Tx, msg, (uint32_t *) CAN_TX_MAILBOX1);
    CAN_EXIT_CRITICAL();
    return err;
}

/**
 * @brief CAN2读取数据
 * @param[out] buf 数据缓冲区
 * @return HAL Status structures definition
 */
HAL_StatusTypeDef CAN2_Receive_Msg(uint8_t *buf) {
    return HAL_CAN_GetRxMessage(&hcan2, CAN_FilterFIFO1, &CAN2_Rx, buf);
}

#endif
#endif
