#ifndef _VCOMCOMM_H_
#define _VCOMCOMM_H_

#include "RMLibHead.h"

RMLIB_CPP_BEGIN

#ifdef HAL_PCD_MODULE_ENABLED

/**
 * @brief VCOMM底层接收函数，放在CDC_Receive_FS函数中调用
 * @param Buf 缓存
 * @param Len 数据长度
 */
void VCOMM_Receive_FS(uint8_t *Buf, uint32_t *Len);

/**
 * @brief VCOMM发送
 * @param fun_code 功能码
 * @param id 消息ID
 * @param data 数据
 * @param len 数据长度
 */
void VCOMM_Transmit(uint8_t fun_code, uint16_t id, uint8_t *data, uint8_t len);

/**
 * @brief VCOMM接收回调,虚函数,使用时重写此函数
 * @param fun_code 功能码
 * @param id 消息ID
 * @param data 消息数据
 * @param len 数据长度
 */
void VCOMM_CallBack(uint8_t fun_code, uint16_t id, uint8_t *data, uint8_t len);

/**
 * @brief VCOMM错误回调函数,虚函数,使用时重写此函数
 *        data和len全为0时是USB状态错误
 *
 * @param data 原始数据指针
 * @param len 原始数据长度
 */
void VCOMM_Error_CallBack(uint8_t *data, uint8_t len);

#endif

RMLIB_CPP_END

#endif
