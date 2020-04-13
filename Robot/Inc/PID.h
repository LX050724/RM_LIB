#ifndef _PID_H_
#define _PID_H_

#define limit(_IN_, _MAX_, _MIN_) \
    if (_IN_ < _MIN_)             \
        _IN_ = _MIN_;             \
    if (_IN_ > _MAX_)             \
        _IN_ = _MAX_;

typedef struct PID_PARAMETER
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

typedef struct PID_PARAMETER_SMIS
{
    float Kp;
    float Ki;
    float Kd;
    float limit; //积分限幅
    float error_now;
    float error_inter;
    float pid_out;
} PID_Smis;

typedef struct PID_INCREASE
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
