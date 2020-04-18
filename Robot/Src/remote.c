#include "remote.h"

RC_Ctl_t RC_CtrlData = {.rc = {1024, 1024, 1024, 1024, 2,
                               1}};                                                 //remote control data

/**
 * @brief 遥控器模式控制回调函数
 * @param rc 摇杆数据结构体
 */
__weak void RemoteControlProcess(Remote *rc) {
    UNUSED(rc);
}

/**
 * @brief 键鼠模式控制回调函数
 * @param mouse 鼠标数据结构体
 * @param key 键盘数据结构体
 * @param Lastkey 上一帧键盘数据结构体
 */
__weak void MouseKeyControlProcess(Mouse *mouse, Key_t key, Key_t Lastkey) {
    UNUSED(mouse);
    UNUSED(key);
}

/**
 * @brief 停止模式控制回调函数
 */
__weak void STOPControlProcess(void) {
    return;
}

/**
 * @brief 遥控器数据归零
 */
void Remote_Zero() {
    RC_CtrlData.rc.ch0 = 1024;
    RC_CtrlData.rc.ch1 = 1024;
    RC_CtrlData.rc.ch2 = 1024;
    RC_CtrlData.rc.ch3 = 1024;
    RC_CtrlData.rc.s1 = 2;
    RC_CtrlData.rc.s2 = 1;
    RC_CtrlData.key.S = 0;
    RC_CtrlData.Lastkey.S = 0;
    RC_CtrlData.mouse.x = 0;
    RC_CtrlData.mouse.y = 0;
    RC_CtrlData.mouse.z = 0;
    RC_CtrlData.mouse.press_r = 0;
    RC_CtrlData.mouse.press_l = 0;
    RC_CtrlData.mouse.last_press_r = 0;
    RC_CtrlData.mouse.last_press_l = 0;
}

/**
 * @brief 遥控器数据接收函数
 * @param RxMsg 遥控器串口原始数据
 */
void Remote_Rx(unsigned char *RxMsg) {
    RC_CtrlData.rc.ch0 = ((int16_t) RxMsg[0] | ((int16_t) RxMsg[1] << 8)) & 0x07FF;
    RC_CtrlData.rc.ch1 = (((int16_t) RxMsg[1] >> 3) | ((int16_t) RxMsg[2] << 5)) & 0x07FF;
    RC_CtrlData.rc.ch2 = (((int16_t) RxMsg[2] >> 6) | ((int16_t) RxMsg[3] << 2) |
                          ((int16_t) RxMsg[4] << 10)) & 0x07FF;
    RC_CtrlData.rc.ch3 = (((int16_t) RxMsg[4] >> 1) | ((int16_t) RxMsg[5] << 7)) & 0x07FF;

    RC_CtrlData.rc.s1 = ((RxMsg[5] >> 4) & 0x000C) >> 2;
    RC_CtrlData.rc.s2 = ((RxMsg[5] >> 4) & 0x0003);

    RC_CtrlData.mouse.x = ((int16_t) RxMsg[6]) | ((int16_t) RxMsg[7] << 8);
    RC_CtrlData.mouse.y = ((int16_t) RxMsg[8]) | ((int16_t) RxMsg[9] << 8);
    RC_CtrlData.mouse.z = ((int16_t) RxMsg[10]) | ((int16_t) RxMsg[11] << 8);

    RC_CtrlData.mouse.press_l = RxMsg[12];
    RC_CtrlData.mouse.press_r = RxMsg[13];

    RC_CtrlData.key.S = ((int16_t) RxMsg[14] | (int16_t) RxMsg[15] << 8);

    switch (RC_CtrlData.rc.s2) {
        case REMOTE_INPUT:
            //遥控器控制模式
            RemoteControlProcess(&(RC_CtrlData.rc));
            break;
        case KEY_MOUSE_INPUT:
            //键鼠控制模式
            MouseKeyControlProcess(&RC_CtrlData.mouse, RC_CtrlData.key.D, RC_CtrlData.Lastkey.D);
            break;
        case STOP:
            STOPControlProcess();
            break;
    }
    RC_CtrlData.mouse.last_press_l = RC_CtrlData.mouse.press_l;
    RC_CtrlData.mouse.last_press_r = RC_CtrlData.mouse.press_r;
    RC_CtrlData.Lastkey.S = RC_CtrlData.key.S;
}
