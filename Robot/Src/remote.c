#include "remote.h"

RC_Ctl_t RC_CtrlData = {.rc = {1024, 1024, 1024, 1024, 2, 1}};     //!<@brief remote control data

__weak void RemoteControlProcess(Remote *rc) {
    UNUSED(rc);
}

__weak void MouseKeyControlProcess(Mouse *mouse, Key_t key, Key_t Lastkey) {
    UNUSED(mouse);
    UNUSED(key);
}

__weak void STOPControlProcess(void) {
    return;
}

void RemoteClear() {
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

void Remote_Rx(unsigned char *RxMsg) {
    RC_CtrlData.rc.ch0 = (RxMsg[0] | (RxMsg[1] << 8)) & 0x07FF;
    RC_CtrlData.rc.ch1 = ((RxMsg[1] >> 3) | (RxMsg[2] << 5)) & 0x07FF;
    RC_CtrlData.rc.ch2 = ((RxMsg[2] >> 6) | (RxMsg[3] << 2) | (RxMsg[4] << 10)) & 0x07FF;
    RC_CtrlData.rc.ch3 = ((RxMsg[4] >> 1) | (RxMsg[5] << 7)) & 0x07FF;

    RC_CtrlData.rc.s1 = (RxMsg[5] >> 4 & 0x000C) >> 2;
    RC_CtrlData.rc.s2 = (RxMsg[5] >> 4 & 0x0003);

    RC_CtrlData.mouse.x = (int16_t)(RxMsg[6] | (RxMsg[7] << 8));
    RC_CtrlData.mouse.y = (int16_t)(RxMsg[8] | (RxMsg[9] << 8));
    RC_CtrlData.mouse.z = (int16_t)(RxMsg[10] | (RxMsg[11] << 8));

    RC_CtrlData.mouse.press_l = RxMsg[12];
    RC_CtrlData.mouse.press_r = RxMsg[13];

    *(uint16_t * ) & (RC_CtrlData.key) = RxMsg[14] | RxMsg[15] << 8;

    switch (RC_CtrlData.rc.s2) {
        case REMOTE_INPUT:
            //遥控器控制模式
            RemoteControlProcess(&(RC_CtrlData.rc));
            break;
        case KEY_MOUSE_INPUT:
            //键鼠控制模式
            MouseKeyControlProcess(&RC_CtrlData.mouse, RC_CtrlData.key, RC_CtrlData.Lastkey);
            break;
        case STOP:
            STOPControlProcess();
            break;
    }
    RC_CtrlData.mouse.last_press_l = RC_CtrlData.mouse.press_l;
    RC_CtrlData.mouse.last_press_r = RC_CtrlData.mouse.press_r;
    RC_CtrlData.Lastkey = RC_CtrlData.key;
}
