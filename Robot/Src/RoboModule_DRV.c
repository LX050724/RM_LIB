#include "RoboModule_DRV.h"
#include "CANDrive.h"
#include "PID.h"
#include "math.h"

#if defined(HAL_CAN_MODULE_ENABLED)

static void (* const SendList[9])(CAN_HandleTypeDef *, RoboModule_Typedef*) = {
        NULL,
        CAN_RoboModule_DRV_OpenLoop_Mode,
        CAN_RoboModule_DRV_Current_Mode,
        CAN_RoboModule_DRV_Velocity_Mode,
        CAN_RoboModule_DRV_Position_Mode,
        CAN_RoboModule_DRV_Velocity_Position_Mode,
        CAN_RoboModule_DRV_Current_Velocity_Mode,
        CAN_RoboModule_DRV_Current_Position_Mode,
        CAN_RoboModule_DRV_Current_Velocity_Position_Mode
};

void CAN_RoboModule_DRV_Reset(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt) {
    uint8_t data[8] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x000, data);
}

void CAN_RoboModule_DRV_Mode_Choice(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt, RoboModule_Mode mode) {
    uint8_t data[8] = {(rmt->mode = mode), 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x001, data);
}

void CAN_RoboModule_DRV_OpenLoop_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt) {
    limit(rmt->ExpPWM, 5000, -5000);
    uint8_t data[8] = {(uint8_t) ((rmt->ExpPWM >> 8) & 0xff),
                       (uint8_t) (rmt->ExpPWM & 0xff),
                       0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x002, data);
}

void CAN_RoboModule_DRV_Current_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt) {
    limit(rmt->ExpPWM, 5000, -5000);
    rmt->ExpPWM = abs(rmt->ExpPWM);

    uint8_t data[8] = {(rmt->ExpPWM >> 8) & 0xff,
                       rmt->ExpPWM & 0xff,
                       (rmt->ExpCurrent >> 8) & 0xff,
                       rmt->ExpCurrent & 0xff,
                       0x55, 0x55, 0x55, 0x55};

    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x003, data);
}

void CAN_RoboModule_DRV_Velocity_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt) {
    limit(rmt->ExpPWM, 5000, -5000);
    rmt->ExpPWM = abs(rmt->ExpPWM);

    uint8_t data[8] = {(rmt->ExpPWM >> 8) & 0xff,
                       rmt->ExpPWM & 0xff,
                       (rmt->ExpVelocity >> 8) & 0xff,
                       rmt->ExpVelocity & 0xff,
                       0x55, 0x55, 0x55, 0x55};

    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x004, data);
}

void CAN_RoboModule_DRV_Position_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt) {
    limit(rmt->ExpPWM, 5000, -5000);
    rmt->ExpPWM = abs(rmt->ExpPWM);

    uint8_t data[8] = {(rmt->ExpPWM >> 8) & 0xff,
                       rmt->ExpPWM & 0xff,
                       0x55,
                       0x55,
                       (rmt->ExpPosition >> 24) & 0xff,
                       (rmt->ExpPosition >> 16) & 0xff,
                       (rmt->ExpPosition >> 8) & 0xff,
                       rmt->ExpPosition & 0xff};

    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x005, data);
}

void CAN_RoboModule_DRV_Velocity_Position_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt) {
    limit(rmt->ExpPWM, 5000, -5000);
    rmt->ExpPWM = abs(rmt->ExpPWM);
    rmt->ExpVelocity = abs(rmt->ExpVelocity);

    uint8_t data[8] = {(rmt->ExpPWM >> 8) & 0xff,
                       rmt->ExpPWM & 0xff,
                       (rmt->ExpVelocity >> 8) & 0xff,
                       rmt->ExpVelocity & 0xff,
                       (rmt->ExpPosition >> 24) & 0xff,
                       (rmt->ExpPosition >> 16) & 0xff,
                       (rmt->ExpPosition >> 8) & 0xff,
                       rmt->ExpPosition & 0xff};

    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x006, data);
}

void CAN_RoboModule_DRV_Current_Velocity_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt) {
    rmt->ExpVelocity = abs(rmt->ExpVelocity);


    uint8_t data[8] = {(rmt->ExpCurrent >> 8) & 0xff,
                       rmt->ExpCurrent & 0xff,
                       (rmt->ExpVelocity >> 8) & 0xff,
                       rmt->ExpVelocity & 0xff,
                       0x55,
                       0x55,
                       0x55,
                       0x55};

    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x007, data);
}

void CAN_RoboModule_DRV_Current_Position_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt) {
    rmt->ExpCurrent = abs(rmt->ExpCurrent);

    uint8_t data[8] = {(rmt->ExpCurrent >> 8) & 0xff,
                       rmt->ExpCurrent & 0xff,
                       0x55,
                       0x55,
                       (rmt->ExpPosition >> 24) & 0xff,
                       (rmt->ExpPosition >> 16) & 0xff,
                       (rmt->ExpPosition >> 8) & 0xff,
                       rmt->ExpPosition & 0xff};

    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x008, data);
}

void CAN_RoboModule_DRV_Current_Velocity_Position_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt) {
    rmt->ExpCurrent = abs(rmt->ExpCurrent);
    rmt->ExpVelocity = abs(rmt->ExpVelocity);

    uint8_t data[8] = {(rmt->ExpCurrent >> 8) & 0xff,
                       rmt->ExpCurrent & 0xff,
                       (rmt->ExpVelocity >> 8) & 0xff,
                       rmt->ExpVelocity & 0xff,
                       (rmt->ExpPosition >> 24) & 0xff,
                       (rmt->ExpPosition >> 16) & 0xff,
                       (rmt->ExpPosition >> 8) & 0xff,
                       rmt->ExpPosition & 0xff};

    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x009, data);
}

void CAN_RoboModule_DRV_Config(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt, uint8_t FbTime, uint8_t Ctl1_Ctl2) {
    if (Ctl1_Ctl2 != 0 && Ctl1_Ctl2 != 1)
        Ctl1_Ctl2 = 0;
    uint8_t data[8] = {FbTime, Ctl1_Ctl2, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x00A, data);
}

void CAN_RoboModule_DRV_Online_Check(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt) {
    uint8_t data[8] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
    CAN_Send_StdDataFrame(hcan, rmt->ID | 0x00f, data);
}

void CAN_RoboModule_DRV_Feedback(RoboModule_Typedef *rmt, uint8_t *data) {
    rmt->Current = (data[0] << 8) | data[1];
    rmt->Velocity = (data[2] << 8) | data[3];
    rmt->Position = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
}

void CAN_RoboModule_DRV_Send(CAN_HandleTypeDef *hcan, RoboModule_Typedef *rmt) {
    SendList[rmt->mode](hcan, rmt);
}

uint8_t CAN_RoboModule_DRV_PositionArrive(RoboModule_Typedef *rmt, uint8_t error) {
    return abs(rmt->ExpPosition - rmt->Position) < error ? 1 : 0;
}

#endif
