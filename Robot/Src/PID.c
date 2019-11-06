#include "PID.h"

void PID_Control(float current /*实际值*/, float expected /*期望值*/, PID *motor_type /*参数*/)
{
	motor_type->error_last = motor_type->error_now;
	motor_type->error_now = expected - current;
	motor_type->error_inter += motor_type->error_now;

	if (motor_type->error_inter > 10000)
		motor_type->error_inter = 10000;
	if (motor_type->error_inter < -10000)
		motor_type->error_inter = -10000;
	motor_type->pid_out = motor_type->Kp * motor_type->error_now + motor_type->Ki * motor_type->error_inter +
						  motor_type->Kd * (motor_type->error_now - motor_type->error_last);
}

float PID_Increment(float current,float expect,PID_ADD *parameter)
{
	parameter->error_now=expect-current;
	
	parameter->increament=parameter->Kp*(parameter->error_now-parameter->error_next)+parameter->Ki*(parameter->error_now)+
	                       parameter->Kd*(parameter->error_now-2*parameter->error_next+parameter->error_last);
	
	parameter->error_last=parameter->error_next;
	parameter->error_next=parameter->error_now;
	
	return parameter->increament;
}

float PID_diff_convert(float init_data, Diff *type)
{
	type->x_now = init_data;

	type->y_now = (int)(type->NUM[0] * type->x_now + type->NUM[1] * type->x_last + type->NUM[2] * type->x_next - type->DEN[1] * type->y_last - type->DEN[2] * type->y_next);

	type->y_next = type->y_last;
	type->y_last = type->y_now;
	type->x_next = type->x_last;
	type->x_last = type->x_now;
	return type->y_now;
}
