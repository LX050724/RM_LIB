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
	float error_now;
	float error_last;
	float error_inter;
	float pid_out;
} PID;

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

typedef struct
{
	float NUM[3];
	float DEN[3];
	float x_now;
	float x_last;
	float x_next;
	float y_now;
	float y_last;
	float y_next;

} Diff;

void PID_Control(float current, float expected, PID *motor_type);
float PID_Increment(float current,float expect,PID_ADD *parameter);
float PID_diff_convert(float init_data, Diff *type);

#endif
