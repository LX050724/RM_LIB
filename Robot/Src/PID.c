#include "PID.h"

void PID_Control(float current /*实际值*/, float expected /*期望值*/, PID *data /*参数*/)
{
	data->error_last = data->error_now;
	data->error_now = expected - current;
	data->error_inter += data->error_now;

	if (data->error_inter > data->limit)
		data->error_inter = data->limit;
	if (data->error_inter < -data->limit)
		data->error_inter = -data->limit;
	data->pid_out = data->Kp * data->error_now + data->Ki * data->error_inter +
						  data->Kd * (data->error_now - data->error_last);
}

float PID_Increment(float current, float expect, PID_ADD *parameter)
{
	parameter->error_now = expect - current;

	parameter->increament = parameter->Kp * (parameter->error_now - parameter->error_next) + parameter->Ki * (parameter->error_now) +
							parameter->Kd * (parameter->error_now - 2 * parameter->error_next + parameter->error_last);

	parameter->error_last = parameter->error_next;
	parameter->error_next = parameter->error_now;

	return parameter->increament;
}

/***********************    带史密斯预估器的位置式PID↓   ************************/
void PID_Control_Smis(float current, float expected, PID_Smis *data, float speed)
{
	data->error_now = expected - current;
	data->error_inter += data->error_now;

	if (data->error_inter > data->limit)
		data->error_inter = data->limit;
	if (data->error_inter < -data->limit)
		data->error_inter = -data->limit;

	data->pid_out = data->Kp * data->error_now + data->Ki * data->error_inter +
						  data->Kd * speed;
}
