/**
 * @file    PrintfDMABuffer.c
 * @author  yao
 * @date    8-Apr-2021
 * @brief   Printf缓冲区模块
 */

#include "PrintfDMABuffer.h"

#if defined(HAL_USART_MODULE_ENABLED) || defined(HAL_UART_MODULE_ENABLED) || defined(HAL_PCD_MODULE_ENABLED)

#if defined(HAL_PCD_MODULE_ENABLED)
#include "usbd_cdc_if.h"

static volatile uint8_t USBTransmit_Lock = 0;
#endif

FILE __stdout = {0};
FILE __stderr = {0};

/**
 * @brief 内部函数，将字符压入缓冲区中
 * @param hpb 缓冲区句柄指针
 * @param ch 字符
 * @return 1：缓冲区满，需要刷新；0：缓冲区未满
 */
static uint8_t PushBuffer(PrintDMABuffer_HandleTypeDef *hpb, int ch);

void _sys_exit(int x) {
    x = x;
}

int fputc(int ch, FILE *f) {
    if ((void *)f->handle != NULL) {
        PrintDMABuffer_HandleTypeDef *hpb = (PrintDMABuffer_HandleTypeDef *) f->handle;
#ifdef __USE_RTOS
        xSemaphoreTake(hpb->mutex, portMAX_DELAY);
#else
        while(hpb->lock == 1);
        hpb->lock = 1;
#endif
#if defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED)
        if (hpb->bufferSize == 1 && hpb->huart != NULL) {
            HAL_UART_Transmit(hpb->huart, (uint8_t *)&ch, 1, 10);
            goto End;
        }

        if (hpb->huart != NULL) {
            while (hpb->huart->gState != HAL_UART_STATE_READY);
            if (PushBuffer(hpb, ch) || ch == '\n') {
                PrintBufferFlush(hpb);
            }
#else
        if (0) {
#endif
#if defined(HAL_PCD_MODULE_ENABLED)
        } else {
            volatile uint32_t count = 0x00000FFF;
            while (USBTransmit_Lock == 1 && count-- > 1);
            if (PushBuffer(hpb, ch) || ch == '\n') {
                PrintBufferFlush(hpb);
            }
#endif
        }
End:
#ifdef __USE_RTOS
        xSemaphoreGive(hpb->mutex);
#else
        hpb->lock = 0;
#endif
    }
    return ch;
}

#ifdef __GNUC__

int _write(int fd, const void *buffer, unsigned int count) {
    FILE *f = NULL;
    switch (fd) {
        case 1:
            f = &__stdout;
            break;
        case 2:
            f = &__stderr;
            break;
        default:
            return -1;
    }
    for (int i = 0; i < count; i++)
        fputc((int) ((char *) buffer)[i], f);
    return count;
}

#endif

FILE PrintBufferRedirect(PrintDMABuffer_HandleTypeDef *hpb) {
    FILE t = {(int) hpb};
    return t;
}

#if defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED)
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
#ifdef __USE_RTOS
    hpb->mutex = xSemaphoreCreateMutex();
#else
    hpb->lock = 0;
#endif
    return RM_SUCCESS;
}
#endif

#if defined(HAL_PCD_MODULE_ENABLED)
RM_Status PrintBufferInit_VCOM(PrintDMABuffer_HandleTypeDef* hpb) {
#if defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED)
    hpb->huart = NULL;
#endif
    hpb->buffer = (uint8_t *) RMLIB_MALLOC(64);
    if (hpb->buffer == NULL) {
        /** Error: Out of memory */
        return RM_ERROR;
    }
    hpb->bufferSize = 64;
    hpb->index = 0;
#ifdef __USE_RTOS
    hpb->mutex = xSemaphoreCreateMutex();
#else
    hpb->lock = 0;
#endif
    return RM_SUCCESS;
}

void PrintBuffer_USBTransmitCplt(void) {
    USBTransmit_Lock = 0;
}
#endif

void PrintBufferDeInit(PrintDMABuffer_HandleTypeDef *hpb) {
    RMLIB_FREE(hpb->buffer);
}

static uint8_t PushBuffer(PrintDMABuffer_HandleTypeDef *hpb, int ch) {
    if (hpb->index < hpb->bufferSize)
        hpb->buffer[hpb->index++] = ch;
    return (hpb->index == hpb->bufferSize) ? 1 : 0;
}

HAL_StatusTypeDef PrintBufferFlush(PrintDMABuffer_HandleTypeDef* hpb) {
    uint16_t size = hpb->index;
    hpb->index = 0;
#if defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED)
    if (hpb->huart != NULL)
        return HAL_UART_Transmit_DMA(hpb->huart, hpb->buffer, size);
#endif
#if defined(HAL_PCD_MODULE_ENABLED)
    extern USBD_HandleTypeDef hUsbDeviceFS;
    if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) {
        switch (CDC_Transmit_FS(hpb->buffer, size)) {
            case USBD_OK:
                USBTransmit_Lock = 1;
                return HAL_OK;
            case USBD_BUSY:
                USBTransmit_Lock = 0;
                return HAL_BUSY;
            case USBD_FAIL:
                USBTransmit_Lock = 0;
                return HAL_ERROR;
        }
    }
#endif
    return HAL_ERROR;
}

#endif
