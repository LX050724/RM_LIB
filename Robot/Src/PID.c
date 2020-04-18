#include "PID.h"

/**
 * @brief 标准位置式PID
 * @param current 实际值
 * @param expected 期望值
 * @param parameter PID参数
 */
void PID_Control(float current, float expected, PID *parameter) {
    parameter->error_last = parameter->error_now;
    parameter->error_now = expected - current;
    parameter->error_inter += parameter->error_now;

    if (parameter->error_inter > parameter->limit)
        parameter->error_inter = parameter->limit;
    if (parameter->error_inter < -parameter->limit)
        parameter->error_inter = -parameter->limit;
    parameter->pid_out = parameter->Kp * parameter->error_now + parameter->Ki * parameter->error_inter +
                         parameter->Kd * (parameter->error_now - parameter->error_last);
}

/**
 * @brief 增量式PID
 * @param current 实际值
 * @param expected 期望值
 * @param parameter PID参数
 * @return PID增量
 */
float PID_Increment(float current, float expect, PID_ADD *parameter) {
    parameter->error_now = expect - current;

    parameter->increament =
            parameter->Kp * (parameter->error_now - parameter->error_next) + parameter->Ki * (parameter->error_now) +
            parameter->Kd * (parameter->error_now - 2 * parameter->error_next + parameter->error_last);

    parameter->error_last = parameter->error_next;
    parameter->error_next = parameter->error_now;

    return parameter->increament;
}

/**
 * @brief 带史密斯预估器的位置式PID
 * @param current 实际值
 * @param expected 期望值
 * @param parameter PID参数
 * @param speed 实际速度
 */
void PID_Control_Smis(float current, float expected, PID_Smis *parameter, float speed) {
    parameter->error_now = expected - current;
    parameter->error_inter += parameter->error_now;

    if (parameter->error_inter > parameter->limit)
        parameter->error_inter = parameter->limit;
    if (parameter->error_inter < -parameter->limit)
        parameter->error_inter = -parameter->limit;

    parameter->pid_out = parameter->Kp * parameter->error_now + parameter->Ki * parameter->error_inter +
                         parameter->Kd * speed;
}
