#ifndef _RM_LIB_
#define _RM_LIB_

#include <RM_LIB_conf.h>

#define __RM_LIB_version__ 20200218

#ifdef RMLIB_CHASSIS_MODULE_ENABLED
	#include "Chassis.h"
#endif

#ifdef RMLIB_KALMAN_MODULE_ENABLED
	#include "kalman.h"
#endif

#ifdef RMLIB_PID_MODULE_ENABLED
	#include "PID.h"
#endif

#ifdef RMLIB_RAMP_MODULE_ENABLED
	#include "ramp.h"
#endif

#ifdef RMLIB_REMOTE_MODULE_ENABLED
	#include "remote.h"
#endif

#ifdef RMLIB_WATCHDOG_MODULE_ENABLED
	#include "WatchDog.h"
#endif

#ifdef RMLIB_CANDRIVE_MODULE_ENABLED
	#ifndef HAL_CAN_MODULE_ENABLED
		#error "CAN must be enabled"
	#endif
	#include "CANDrive.h"
#endif

#ifdef RMLIB_MOTOR_MODULE_ENABLED
	#ifndef RMLIB_CANDRIVE_MODULE_ENABLED
		#error "CANDRIVE_MODULE must be enabled"
	#endif
	#include "motor.h"
#endif

#ifdef RMLIB_KALMANII_MODULE_ENABLED
	#if defined(ARM_MATH_CM4) || defined(ARM_MATH_CM3)
		#include "kalmanII.h"
	#else
		#error "DSP Libary must be enabled"
	#endif
#endif

#endif
