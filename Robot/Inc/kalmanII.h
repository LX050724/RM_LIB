#ifndef _KALMANII_H
#define _KALMANII_H

#if defined(STM32F407xx) || defined(STM32F405xx) || defined(STM32F427xx)
	#include <stm32f4xx.h>
#elif defined(STM32F303xx) || defined(STM32F334xx)
	#include <stm32f3xx.h>
#elif defined(STM32F103xx)
	#include <stm32f1xx.h>
#endif

#include "arm_math.h"

#define mat arm_matrix_instance_f32
#define mat_init arm_mat_init_f32
#define mat_add arm_mat_add_f32
#define mat_sub arm_mat_sub_f32
#define mat_mult arm_mat_mult_f32
#define mat_trans arm_mat_trans_f32
#define mat_inv arm_mat_inverse_f32

typedef struct
{
  float raw_value;
  float filtered_value[2];
  float xhat_data[2], xhatminus_data[2], z_data[2], Pminus_data[4], K_data[4];
  float P_data[4];
  float AT_data[4], HT_data[4];
  float A_data[4];
  float H_data[4];
  float Q_data[4];
  float R_data[4];
  float E_data[4];
	
	struct kalman_filtercore{
    float raw_value;
    float filtered_value[2];
    mat xhat, xhatminus, z, A, H, AT, HT, Q, R, P, Pminus, K, E;
  }kalman;
} kalman_filterII_t;

void kalmanInitII(kalman_filterII_t *I);
float* KalmanFilterII(kalman_filterII_t *I, float signal1, float signal2);

#endif
