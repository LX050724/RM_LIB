#ifndef _RM_LIB_
#define _RM_LIB_

/*DSP库添加方法，添加..\Drivers\CMSIS\Lib\ARM\arm_cortexM4lf_math.lib
 *添加全局宏定义 ARM_MATH_CM4 __TARGET_FPU_VFP
 * */
#if 1
	#include "Chassis.h"
#endif

#if 1
	#include "filter.h"
#endif

#if 1
	#include "kalman.h"
#endif

#if 1
	#include "motor.h"
#endif

#if 1
	#include "PID.h"
#endif

#if 1
	#include "ramp.h"
#endif

#if 1
	#include "remote.h"
#endif

#if 1
	#include "test_can.h"
#endif

#if 1
	#include "WatchDog.h"
#endif

#endif
