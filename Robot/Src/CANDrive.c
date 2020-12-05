#include "CANDrive.h"

#ifdef HAL_CAN_MODULE_ENABLED

CAN_RxHeaderTypeDef CAN1_Rx;
CAN_TxHeaderTypeDef CAN1_Tx;
uint8_t CAN1_buff[8];

#ifdef CAN2_SUPPORT
CAN_RxHeaderTypeDef CAN2_Rx;
CAN_TxHeaderTypeDef CAN2_Tx;
uint8_t CAN2_buff[8];
#endif


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
        canfilter.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    }
#endif

    /* 激活过滤器 */
    canfilter.FilterActivation = ENABLE;
    HAL_CAN_ConfigFilter(hcan, &canfilter);
}

HAL_StatusTypeDef CAN1_Send_Msg(uint32_t StdId, uint8_t *msg) {
    RMLIB_ENTER_CRITICAL();
    uint32_t mailbox;

    /* 标准标识符 */
    CAN1_Tx.StdId = StdId;

    /* 使用标准帧 */
    CAN1_Tx.IDE = CAN_ID_STD;

    /* 数据帧 */
    CAN1_Tx.RTR = CAN_RTR_DATA;

    CAN1_Tx.DLC = 8;
    CAN1_Tx.TransmitGlobalTime = DISABLE;

    HAL_StatusTypeDef err = HAL_CAN_AddTxMessage(&hcan1, &CAN1_Tx, msg, &mailbox);
    RMLIB_EXIT_CRITICAL();
    return err;
}

#ifdef CAN2_SUPPORT

HAL_StatusTypeDef CAN2_Send_Msg(uint32_t StdId, uint8_t *msg) {
    RMLIB_ENTER_CRITICAL();
    uint32_t mailbox;

    /* 标准标识符 */
    CAN2_Tx.StdId = StdId;

    /* 使用标准帧 */
    CAN2_Tx.IDE = CAN_ID_STD;

    /* 数据帧 */
    CAN2_Tx.RTR = CAN_RTR_DATA;

    CAN2_Tx.DLC = 8;
    CAN2_Tx.TransmitGlobalTime = DISABLE;
    HAL_StatusTypeDef err = HAL_CAN_AddTxMessage(&hcan2, &CAN2_Tx, msg, &mailbox);
    RMLIB_EXIT_CRITICAL();
    return err;
}
#endif
#endif
