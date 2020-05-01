/**
 * @file    PID.h
 * @author  yao
 * @date    1-May-2020
 * @brief   PID模块头文件
 */

#ifndef _PID_H_
#define _PID_H_

/**
 * @brief 限幅宏函数
 * @param IN 限幅变量
 * @param MAX 最大值
 * @param MIN 最小值
 */
#define limit(IN, MAX, MIN) \
    if (IN < MIN)           \
        IN = MIN;           \
    if (IN > MAX)           \
        IN = MAX

/**
 * @brief 标准位置式PID参数
 */
typedef struct __PID {
    float Kp;           //!<@brief 比例系数
    float Ki;           //!<@brief 积分系数
    float Kd;           //!<@brief 微分系数
    float limit;        //!<@brief 积分限幅
    float error_now;    //!<@brief 当前误差
    float error_last;   //!<@brief 上一次误差
    float error_inter;  //!<@brief 误差积分
    float pid_out;      //!<@brief PID输出
} PID;

/**
 * @brief 带史密斯预估器的位置式PID参数
 */
typedef struct __PID_Smis {
    float Kp;           //!<@brief 比例系数
    float Ki;           //!<@brief 积分系数
    float Kd;           //!<@brief 微分系数
    float limit;        //!<@brief 积分限幅
    float error_now;    //!<@brief 当前误差
    float error_inter;  //!<@brief 误差积分
    float pid_out;      //!<@brief PID输出
} PID_Smis;

/**
 * @brief 增量式PID参数
 */
typedef struct __PID_ADD {
    float Kp;           //!<@brief 比例系数
    float Ki;           //!<@brief 积分系数
    float Kd;           //!<@brief 微分系数
    float error_now;    //!<@brief 当前误差
    float error_next;   //!<@brief 上一次误差
    float error_last;   //!<@brief 上上次误差
    float increament;   //!<@brief PID增量
} PID_ADD;

void PID_Control(float current, float expected, PID *data);

void PID_Control_Smis(float current, float expected, PID_Smis *data, float speed);

float PID_Increment(float current, float expect, PID_ADD *parameter);

#endif
