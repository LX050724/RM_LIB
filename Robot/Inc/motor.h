/**
 * @file    motor.h
 * @author  yao
 * @date    1-May-2020
 * @brief   电机驱动模块头文件
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "RMLibHead.h"

RMLIB_CPP_BEGIN

#ifdef HAL_CAN_MODULE_ENABLED
#include "CANDrive.h"

#define RM3508_LIMIT 16384  //!<@brief RM3508的输出限幅
#define GM6020_LIMIT 30000  //!<@brief GM6020的输出限幅
#define RM3510_LIMIT 32767  //!<@brief RM3510的输出限幅
#define GM3510_LIMIT 29000  //!<@brief GM3510的输出限幅
#define M2006_LIMIT  10000  //!<@brief M2006 的输出限幅
#define RM6623_LIMIT 32767  //!<@brief RM6623的输出限幅(找不到了)

/**
 * @brief RM3508电机数据结构体
 */
typedef struct {
    uint16_t MchanicalAngle;    //!<@brief 机械角度
    int16_t Speed;              //!<@brief 转速
    int16_t TorqueCurrent;      //!<@brief 转矩电流
    uint8_t temp;               //!<@brief 温度
    float Power;                //!<@brief 功率
    uint16_t LsatAngle;         //!<@brief 上一次的机械角度
    int16_t r;                  //!<@brief 圈数
    int32_t Angle;              //!<@brief 连续化机械角度 @warning 由于启动时角度不确定，启动时连续化角度可能有一圈的偏差
    float Angle_DEG;            //!<@brief 连续化角度制角度 @warning 由于启动时角度不确定，启动时连续化角度可能有一圈的偏差
    struct PowerCOF_s {
        float ss;               //!<@brief 速度平方项系数
        float sc;               //!<@brief 速度,转矩电流乘积项系数
        float cc;               //!<@brief 转矩电流平方项系数
        float constant;         //!<@brief 常量
    } PowerCOF;                 //!<@brief 计算功率所用的系数,由MATLAB拟合
} RM3508_TypeDef;

/**
 * @brief GM6020电机数据结构体
 */
typedef struct {
    uint16_t MchanicalAngle;    //!<@brief 机械角度
    int16_t Speed;              //!<@brief 转速
    int16_t TorqueCurrent;      //!<@brief 转矩电流
    uint8_t temp;               //!<@brief 温度
    uint16_t LsatAngle;         //!<@brief 上一次的机械角度
    int16_t r;                  //!<@brief 圈数
    int32_t Angle;              //!<@brief 连续化机械角度 @warning 由于启动时角度不确定，启动时连续化角度可能有一圈的偏差
    float Angle_DEG;            //!<@brief 连续化角度制角度 @warning 由于启动时角度不确定，启动时连续化角度可能有一圈的偏差
} GM6020_TypeDef;

/**
 * @brief RM3510电机数据结构体
 */
typedef struct {
    uint16_t MchanicalAngle;    //!<@brief 机械角度    
    int16_t Speed;              //!<@brief 转速
} RM3510_TypeDef;

/**
 * @brief GM3510电机数据结构体
 */
typedef struct {
    uint16_t MchanicalAngle;    //!<@brief 机械角度
    int16_t OutputTorque;       //!<@brief 输出扭矩
    uint16_t LsatAngle;         //!<@brief 上一次的机械角度
    int16_t r;                  //!<@brief 圈数
    int32_t Angle;              //!<@brief 连续化机械角度 @warning 由于启动时角度不确定，启动时连续化角度可能有一圈的偏差
    float Angle_DEG;            //!<@brief 连续化角度制角度 @warning 由于启动时角度不确定，启动时连续化角度可能有一圈的偏差
} GM3510_TypeDef;

/**
 * @brief M2006电机数据结构体
 */
typedef struct {
    uint16_t MchanicalAngle;    //!<@brief 机械角度
    int16_t Speed;              //!<@brief 转速
    uint16_t LsatAngle;         //!<@brief 上一次的机械角度
    int16_t r;                  //!<@brief 圈数
    int32_t Angle;              //!<@brief 连续化机械角度 @warning 由于启动时角度不确定，启动时连续化角度可能有一圈的偏差
    float Angle_DEG;            //!<@brief 连续化角度制角度 @warning 由于启动时角度不确定，启动时连续化角度可能有一圈的偏差
} M2006_TypeDef;

/**
 * @brief RM6623电机数据结构体
 */
typedef struct {
    uint16_t MchanicalAngle;    //!<@brief 机械角度
    int16_t TorqueCurrent;      //!<@brief 转矩电流
    int16_t SetTorqueCurrent;   //!<@brief 设定转矩电流
    uint16_t LsatAngle;         //!<@brief 上一次的机械角度
    int16_t r;                  //!<@brief 圈数
    int32_t Angle;              //!<@brief 连续化机械角度 @warning 由于启动时角度不确定，启动时连续化角度可能有一圈的偏差
    float Angle_DEG;            //!<@brief 连续化角度制角度 @warning 由于启动时角度不确定，启动时连续化角度可能有一圈的偏差
} RM6623_TypeDef;

/**
 * @brief RM6623数据接收
 * @param[out] Dst RM6623电机数据结构体指针
 * @param[in] Data CAN数据帧指针
 */
void RM6623_Receive(RM6623_TypeDef *Dst, uint8_t *Data);

/**
 * @brief RM3510数据接收
 * @param[out] Dst RM3510电机数据结构体指针
 * @param[in] Data CAN数据帧指针
 */
void RM3510_Receive(RM3510_TypeDef *Dst, uint8_t *Data);

/**
 * @brief 设置RM3508功率计算参数
 * @param[out] Dst RM3510电机数据结构体指针
 * @param[in] cc 电流平方项系数
 * @param[in] sc 电流,转速乘积项系数
 * @param[in] ss 转速平方项系数
 * @param[in] constant 常数项
 */
static inline void RM3508_SetPowerCOF(RM3508_TypeDef *Dst, float cc, float sc, float ss, float constant) {
    Dst->PowerCOF.cc = cc;
    Dst->PowerCOF.sc = sc;
    Dst->PowerCOF.ss = ss;
    Dst->PowerCOF.constant = constant;
}

/**
 * @brief RM3508数据接收
 * @param[out] Dst RM3508电机数据结构体指针
 * @param[in] Data CAN数据帧指针
 */
void RM3508_Receive(RM3508_TypeDef *Dst, uint8_t *Data);

/**
 * @brief GM6020数据接收
 * @param[out] Dst GM6020电机数据结构体指针
 * @param[in] Data CAN数据帧指针
 */

void GM6020_Receive(GM6020_TypeDef *Dst, uint8_t *Data);

/**
 * @brief M2006数据接收
 * @param[out] Dst M2006电机数据结构体指针
 * @param[in] Data CAN数据帧指针
 */
void M2006_Receive(M2006_TypeDef *Dst, uint8_t *Data);

/**
 * @brief GM3510数据接收
 * @param[out] Dst GM3510电机数据结构体指针
 * @param[in] Data CAN数据帧指针
 */
void GM3510_Receive(GM3510_TypeDef *Dst, uint8_t *Data);


/**
 * @brief 发送电机控制信号
 * @param hcan CAN句柄
 * @param[in] StdId 标准帧ID
 * @param[in] Data 电机控制信号数组指针
 * @return HAL Status structures definition
 */
HAL_StatusTypeDef MotorSend(CAN_HandleTypeDef *hcan, uint32_t StdId, int16_t *Data);

/**
 * @brief 寻找最短到达路径
 * @details
 *      <tr>输入期望值和电机机械角度，函数会将期望值处理成为机械角度为中心的值，并且是绝对的</p>
 *      其中Exp_是期望值的对称点
 *      归中方向示意图:
 *
 *          || <<<<< | >>>>>>>>>>>> | <<<<< ||
 *          0       Exp_           Exp     8191
 *          || >>>>> | <<<<<<<<<<<< | >>>>> ||
 *          0       Exp            Exp_    8191
 *
 *      使用方法：
 *          将返回值直接作为PID期望，实际值为电机编码值</tr>
 * @param[in] Mch 电机机械角度
 * @param[in] Exp 期望值
 * @return 映射过的期望值
 */
int16_t QuickCentering(uint16_t Mch, uint16_t Exp);

#endif

RMLIB_CPP_END

#endif
