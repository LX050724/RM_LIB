#include "Chassis.h"

#define Driection 2

void Chassis_Speed_Ref_Zero(ChassisSpeed_Ref_t* ref)
{
	ref->forward_back_ref = 0;
	ref->left_right_ref = 0;
	ref->rotate_ref = 0;
}

void PID_Expect(Chassis_Motor_PID_Expect* motor,ChassisSpeed_Ref_t* ref) //得到底盘电机所期望的速度
{
	#if Driection == 1
	motor->Chassis_Motor_PID_Expect_1 = -ref->forward_back_ref -
								   ref->left_right_ref + ref->rotate_ref;

	motor->Chassis_Motor_PID_Expect_2 = ref->forward_back_ref -
								   ref->left_right_ref + ref->rotate_ref;

	motor->Chassis_Motor_PID_Expect_4 = ref->forward_back_ref +
								   ref->left_right_ref + ref->rotate_ref;

	motor->Chassis_Motor_PID_Expect_3 = -ref->forward_back_ref +
												   ref->left_right_ref + ref->rotate_ref;
	#elif Driection == 2
	motor->Chassis_Motor_PID_Expect_3 = -ref->forward_back_ref -
								   ref->left_right_ref + ref->rotate_ref;

	motor->Chassis_Motor_PID_Expect_2 = ref->forward_back_ref -
								   ref->left_right_ref + ref->rotate_ref;

	motor->Chassis_Motor_PID_Expect_1 = ref->forward_back_ref +
								   ref->left_right_ref + ref->rotate_ref;

	motor->Chassis_Motor_PID_Expect_4 = -ref->forward_back_ref +
												   ref->left_right_ref + ref->rotate_ref;
	#elif Driection == 3
	
	#elif Driection == 4
	
	#endif
}
