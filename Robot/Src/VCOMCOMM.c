#include "VCOMCOMM.h"

#ifdef HAL_PCD_MODULE_ENABLED

#include "usbd_cdc_if.h"
#include "CRC.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

void VCOMM_Receive_FS(uint8_t *Buf, uint32_t *Len) {
    if (Buf[0] == 0x5a) {
        uint8_t fun = Buf[1];
        uint16_t id = *((uint16_t * )(Buf + 2));
        uint16_t len = *((uint16_t * )(Buf + 4));
        if (len + 8 != *Len) {
            VCOMM_Error_CallBack(Buf, (uint8_t)(*Len));
            return;
        }
        uint16_t crc = *((uint16_t * )(Buf + 6 + len));
        if (len == 0 || crc == Verify_CRC16_Check_Sum(Buf + 6, len)) {
            VCOMM_CallBack(fun, id, Buf + 6, len);
        } else VCOMM_Error_CallBack(Buf, (uint8_t)(*Len));
    } else VCOMM_Error_CallBack(Buf, (uint8_t)(*Len));
}

void VCOMM_Transmit(uint8_t fun_code, uint16_t id, uint8_t *data, uint8_t len) {
    if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) {
        if (len > 64 - 8)
            return;
        uint8_t buff[64] = {0x5a, fun_code};
        *((uint16_t * )(buff + 2)) = id;
        *((uint16_t * )(buff + 4)) = len;
        memcpy(buff + 6, data, len);
        *((uint16_t * )(buff + 6 + len)) = (len == 0) ? 0 : Verify_CRC16_Check_Sum(data, len);
        CDC_Transmit_FS(buff, len + 8);
    } else VCOMM_Error_CallBack(NULL, 0);
}

__weak void VCOMM_CallBack(uint8_t fun_code, uint16_t id, uint8_t *data, uint8_t len) {
    UNUSED(fun_code);
    UNUSED(id);
    UNUSED(data);
    UNUSED(len);
}

__weak void VCOMM_Error_CallBack(uint8_t *data, uint8_t len) {
    UNUSED(data);
    UNUSED(len);
}

#endif
