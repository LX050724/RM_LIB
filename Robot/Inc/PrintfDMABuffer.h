/**
 * @file    PrintfDMABuffer.h
 * @author  yao
 * @date    8-Apr-2021
 * @brief   Printf缓冲区模块头文件
 */

#ifndef _PRINTF_DMA_BUFFER_H_
#define _PRINTF_DMA_BUFFER_H_

#include "RMLibHead.h"
#include "RMQueue.h"
#include "stdio.h"

RMLIB_CPP_BEGIN

/**
 * @brief 文件结构体
 */
struct __FILE {
    int handle;
};

/**
 * @brief 缓冲区句柄结构体
 */
typedef struct {
    UART_HandleTypeDef *huart;     //!<@brief 串口句柄指针
    uint16_t index;                //!<@brief 缓冲区尾部序号
    uint16_t bufferSize;           //!<@brief 缓冲区大小
    uint8_t* buffer;               //!<@brief 缓冲区指针
} PrintDMABuffer_HandleTypeDef;

/**
 * @brief 初始化缓冲区句柄
 * @details 首先检查huart是否是NULL，如果是，返回错误
 *          不是NULL继续检查缓冲区长度，详见bufferLen
 *          缓冲区自动刷新两种情况：缓冲区满、遇到换行符
 * @param hpb 缓冲区句柄指针
 * @param huart 重定向目的串口
 * @param bufferLen 缓冲区长度
 *                  如果缓冲区长度 <= 1，则不使用缓冲区，直接使用串口普通发送输出，也不会检查串口有没有启用TxDMA
 *                  如果缓冲区长度 > 1，程序会分配相应大小的缓冲区，并且会检查串口有没有启用TxDMA
 * @return 是否初始化成功
 */
RM_Status PrintBufferInit(PrintDMABuffer_HandleTypeDef* hpb, UART_HandleTypeDef *huart, uint32_t bufferLen);

/**
 * @brief 销毁printf缓冲区
 * @warning 注意手动销毁以及重定向的{@link FILE}对象，防止野指
 * @param hpb
 */
void PrintBufferDeInit(PrintDMABuffer_HandleTypeDef* hpb);

/**
 * @brief 刷新缓冲区
 * @details 将缓冲区内的内容通过串口DMA发送
 * @param hpb 缓冲区句柄指针
 * @return DMA是否成功启动
 */
HAL_StatusTypeDef PrintBufferFlush(PrintDMABuffer_HandleTypeDef* hpb);

/**
 * @brief 将文件指针重定向到指定的串口缓冲区
 * @param hpb 缓冲区句柄指针
 * @return FILE对象
 */
FILE PrintBufferRedirect(PrintDMABuffer_HandleTypeDef* hpb);

extern FILE __stdout; //!<@brief 标准输入流对象
extern FILE __stderr; //!<@brief 标准输出流对象

RMLIB_CPP_END

#endif
