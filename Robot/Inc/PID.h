#ifndef _PID_H_
#define _PID_H_

/**
 * @brief 限幅宏函数
 * @param _IN_ 限幅变量
 * @param _MAX_ 最大值
 * @param _MIN_ 最小值
 */
#define limit(_IN_, _MAX_, _MIN_) \
    if (_IN_ < _MIN_)             \
        _IN_ = _MIN_;             \
    if (_IN_ > _MAX_)             \
        _IN_ = _MAX_

/**
 * @brief 标准位置式PID参数
 */
typedef struct __PID
{
    float Kp;
    float Ki;
    float Kd;
    float limit; //积分限幅
    float error_now;
    float error_last;
    float error_inter;
    float pid_out;
} PID;

/**
 * @brief 带史密斯预估器的位置式PID参数
 */
typedef struct __PID_Smis
{
    float Kp;
    float Ki;
    float Kd;
    float limit; //积分限幅
    float error_now;
    float error_inter;
    float pid_out;
} PID_Smis;

/**
 * @brief 增量式PID参数
 */
typedef struct __PID_ADD
{
    float Kp;
    float Ki;
    float Kd;
    float error_now;
    float error_next;
    float error_last;
    float increament;
} PID_ADD;

void PID_Control(float current, float expected, PID *data);
void PID_Control_Smis(float current, float expected, PID_Smis *data, float speed);
float PID_Increment(float current, float expect, PID_ADD *parameter);

#endif
