/**
 * @file    remote.h
 * @author  yao
 * @date    1-May-2020
 * @brief   遥控器模块头文件
 */

#ifndef __remote_H
#define __remote_H

#include "RMLibHead.h"

RMLIB_CPP_BEGIN

#define RC_FRAME_LENGTH 18u
#define REMOTE_CONTROLLER_STICK_OFFSET 1024

/**
 * @brief 摇杆数据结构体
 */
typedef struct {
    int16_t ch0;  //!<@brief 摇杆通道0数据
    int16_t ch1;  //!<@brief 摇杆通道1数据
    int16_t ch2;  //!<@brief 摇杆通道2数据
    int16_t ch3;  //!<@brief 摇杆通道3数据
    int8_t s1;    //!<@brief 开关1状态
    int8_t s2;    //!<@brief 开关2状态
} Remote;

/**
 * @brief 鼠标数据结构体
 */
typedef struct {
    int16_t x;              //!<@brief 鼠标x轴速度
    int16_t y;              //!<@brief 鼠标y轴速度
    int16_t z;              //!<@brief 鼠标z轴速度
    uint8_t last_press_l;   //!<@brief 上一次左键状态
    uint8_t last_press_r;   //!<@brief 上一次右键状态
    uint8_t press_l;        //!<@brief 左键状态
    uint8_t press_r;        //!<@brief 右键状态
} Mouse;

/**
 * @brief 键盘数据结构体
 * @details 每一个成员对应其按键
 */
typedef struct {
    uint16_t W: 1;
    uint16_t S: 1;
    uint16_t A: 1;
    uint16_t D: 1;
    uint16_t Shift: 1;
    uint16_t Ctrl: 1;
    uint16_t Q: 1;
    uint16_t E: 1;
    uint16_t R: 1;
    uint16_t F: 1;
    uint16_t G: 1;
    uint16_t Z: 1;
    uint16_t X: 1;
    uint16_t C: 1;
    uint16_t V: 1;
    uint16_t B: 1;
} Key_t;

/**
 * @brief 遥控器整体数据结构体
 */
typedef struct {
    Remote rc;        //!<@brief 遥控器数据
    Mouse mouse;      //!<@brief 鼠标数据
    Key_t key;        //!<@brief 键盘数据
    Key_t Lastkey;    //!<@brief 上一帧键盘数据
} RC_Ctl_t;

/**
 * @brief 输入模式枚举
 */
typedef enum {
    REMOTE_INPUT = 1,        //!<@brief 遥控器输入
    KEY_MOUSE_INPUT = 3,     //!<@brief 键盘输入
    STOP = 2,                //!<@brief 急停模式
} InputMode_e;

extern RC_Ctl_t RC_CtrlData;

/**
 * @brief 遥控器数据接收函数
 * @param[in] RxMsg 遥控器串口原始数据
 */
void Remote_Rx(unsigned char *RxMsg);

/**
 * @brief 遥控器数据归零
 */
void RemoteClear(void);

/**
 * @brief 遥控器模式控制回调函数
 * @param[in] rc 摇杆数据结构体
 */
void RemoteControlProcess(Remote *rc);

/**
 * @brief 键鼠模式控制回调函数
 * @param[in] mouse 鼠标数据结构体
 * @param[in] key 键盘数据结构体
 * @param[in] Lastkey 上一帧键盘数据结构体
 */
void MouseKeyControlProcess(Mouse *mouse, Key_t key, Key_t Lastkey);

/**
 * @brief 停止模式控制回调函数
 */
void STOPControlProcess(void);

RMLIB_CPP_END

#endif
