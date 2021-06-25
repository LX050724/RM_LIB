/**
 * @file RoboModule_DRV.h
 * @author  yao
 * @date 2021年6月25日
 * @brief RoboModule直流电机驱动器驱动代码
 */

#ifndef __ROBOMODULE_DRV__
#define __ROBOMODULE_DRV__

#include "RMLibHead.h"

#if defined(HAL_CAN_MODULE_ENABLED)

typedef enum {
    OpenLoop_Mode = 0x01,                       //!<@brief 开环模式
    Current_Mode = 0x02,                        //!<@brief 电流模式
    Velocity_Mode = 0x03,                       //!<@brief 速度模式
    Position_Mode = 0x04,                       //!<@brief 位置模式
    Velocity_Position_Mode = 0x05,              //!<@brief 速度位置模式
    Current_Velocity_Mode = 0x06,               //!<@brief 电流速度模式
    Current_Position_Mode = 0x07,               //!<@brief 电流位置模式
    Current_Velocity_Position_Mode = 0x08,      //!<@brief 电流速度位置模式
} RoboModule_Mode;

/**
 * 驱动器结构体
 */
typedef struct {
    RoboModule_Mode mode;                       //!<@brief 电机模式
    uint16_t ID;                                //!<@brief 电机ID

    int16_t ExpPWM;                             //!<@brief 期望PWM宽度
    int16_t ExpCurrent;                         //!<@brief 期望电流
    int16_t ExpVelocity;                        //!<@brief 期望速度
    int32_t ExpPosition;                        //!<@brief 期望位置

    int16_t Current;                            //!<@brief 反馈当前电流
    int16_t Velocity;                           //!<@brief 反馈当前速度
    int32_t Position;                           //!<@brief 反馈当前位置
} RoboModule_Typedef;

/**
 * 复位指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_Reset(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt);

/**
 * 模式选择指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 * @param Mode 选择的模式
 */
void CAN_RoboModule_DRV_Mode_Choice(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt, RoboModule_Mode mode);

/**
 * 开环模式下的数据指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_OpenLoop_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt);

/**
 * 电流模式下的数据指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_Current_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt);

/**
 * 速度模式下的数据指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_Velocity_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt);

/**
 * 位置模式下的数据指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_Position_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt);

/**
 * 速度位置模式下的数据指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_Velocity_Position_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt);

/**
 * 电流速度模式下的数据指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_Current_Velocity_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt);

/**
 * 电流位置模式下的数据指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_Current_Position_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt);

/**
 * 电流速度位置模式下的数据指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_Current_Velocity_Position_Mode(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt);
/**
 * 配置指令
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 * @param Temp_Time 设置反馈周期，的取值范围: 0 ~ 255，为0时候，为关闭电流速度位置反馈功能
 * @param Ctl1_Ctl2 启用左右限位
 * @warning Ctl1，Ctl2的功能仅存在于102 301，其余版本驱动器，Ctl1_Ctl2 = 0 即可
 */
void CAN_RoboModule_DRV_Config(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt, uint8_t FbTime, uint8_t Ctl1_Ctl2);

/**
 * 在线检测
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_Online_Check(CAN_HandleTypeDef *hcan, RoboModule_Typedef* rmt);

/**
 * 驱动器反馈数据接收
 * @param fb 电机反馈数据结构体
 * @param data CAN原始数据
 */
void CAN_RoboModule_DRV_Feedback(RoboModule_Typedef *fb, uint8_t *data);

/**
 * 驱动器发送函数
 * @param hcan CAN句柄
 * @param rmt 驱动器对象
 */
void CAN_RoboModule_DRV_Send(CAN_HandleTypeDef *hcan, RoboModule_Typedef *rmt);

/**
 * 判断电机是否到位
 * @param rmt 驱动器对象
 * @param error 允许的误差
 * @return 0: 没有到达指定位置, 1:到达指定位置
 */
uint8_t CAN_RoboModule_DRV_PositionArrive(RoboModule_Typedef *rmt, uint8_t error);

#endif

#endif
