/**
 * @file    PrintfDMABuffer.c
 * @author  yao
 * @date    8-Apr-2021
 * @brief   Printf缓冲区模块
 */

#include "PrintfDMABuffer.h"

FILE __stdout = { NULL };
FILE __stderr = { NULL };

/**
 * @brief 内部函数，将字符压入缓冲区中
 * @param hpb 缓冲区句柄指针
 * @param ch 字符
 * @return 1：缓冲区满，需要刷新；0：缓冲区未满
 */
static uint8_t PushBuffer(PrintDMABuffer_HandleTypeDef* hpb, int ch);

void _sys_exit(int x) {
    x = x;
}

int fputc(int ch, FILE *f) {
    if (f->handle != NULL) {
        RMLIB_ENTER_CRITICAL();
        PrintDMABuffer_HandleTypeDef *hpb = (PrintDMABuffer_HandleTypeDef *) f->handle;
        if (hpb->bufferSize == 1) {
            HAL_UART_Transmit(hpb->huart, (uint8_t *)&ch, 1, 10);
        } else {
#ifdef __USE_RTOS
            if (hpb->huart->gState != HAL_UART_STATE_READY) {
                RMLIB_EXIT_CRITICAL();
                while(hpb->huart->gState != HAL_UART_STATE_READY);
                RMLIB_ENTER_CRITICAL();
            }
#else
            while (hpb->huart->gState != HAL_UART_STATE_READY);
#endif
            uint8_t f = PushBuffer(hpb, ch);
            if (ch == '\n' || f)
                PrintBufferFlush(hpb);
            RMLIB_EXIT_CRITICAL();
        }
    }
    return ch;
}

FILE PrintBufferRedirect(PrintDMABuffer_HandleTypeDef* hpb) {
    FILE t = {(int) hpb};
    return t;
}

RM_Status PrintBufferInit(PrintDMABuffer_HandleTypeDef* hpb, UART_HandleTypeDef *huart, uint32_t bufferLen) {
    if (huart == NULL) {
        /** Error: huart is Null*/
        return RM_ERROR;
    }
    if (bufferLen <= 1) {
        hpb->bufferSize = 1;
        hpb->buffer = NULL;
    } else {
        if (huart->hdmatx == NULL) {
            /** Error: UART Tx DMA Is not enabled */
            return RM_ERROR;
        }
        hpb->buffer = (uint8_t *) RMLIB_MALLOC(bufferLen);
        if (hpb->buffer == NULL) {
            /** Error: Out of memory */
            return RM_ERROR;
        }
        hpb->bufferSize = bufferLen;
    }
    hpb->index = 0;
    hpb->huart = huart;
    return RM_SUCCESS;
}

void PrintBufferDeInit(PrintDMABuffer_HandleTypeDef* hpb) {
    RMLIB_FREE(hpb->buffer);
}

static uint8_t PushBuffer(PrintDMABuffer_HandleTypeDef* hpb, int ch) {
    if(hpb->index < hpb->bufferSize)
        hpb->buffer[hpb->index++] = ch;
    return (hpb->index == hpb->bufferSize) ? 1 : 0;
}

HAL_StatusTypeDef PrintBufferFlush(PrintDMABuffer_HandleTypeDef* hpb) {
    uint16_t size = hpb->index;
    hpb->index = 0;
    return HAL_UART_Transmit_DMA(hpb->huart, hpb->buffer, size);
}


