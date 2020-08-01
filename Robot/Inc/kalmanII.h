/**
 * @file    kalmanII.h
 * @author  yao
 * @date    1-May-2020
 * @brief   二阶卡尔曼滤波器模块头文件
 */

#ifndef _KALMANII_H
#define _KALMANII_H

#if defined(STM32F407xx) || defined(STM32F405xx) || defined(STM32F427xx)
#include <stm32f4xx.h>
#elif defined(STM32F303xx) || defined(STM32F334xx)
#include <stm32f3xx.h>
#elif defined(STM32F103xx)
#include <stm32f1xx.h>
#endif

#if !defined(ARM_MATH_CM4) && !defined(ARM_MATH_CM3)
#error "No DSP library support"
#endif

#include "arm_math.h"

/**
 * @brief 二阶卡尔曼滤波器
 */
typedef struct __kalman_filterII_t {
    float raw_value;
    float filtered_value[2];
    float xhat_data[2], xhatminus_data[2], z_data[2], Pminus_data[4], K_data[4];
    float P_data[4];
    float AT_data[4], HT_data[4];
    float A_data[4];
    float H_data[4];
    float Q_data[4];
    float R_data[4];

    /**
     * @brief 滤波器核心
     */
    struct kalman_filtercore {
        float raw_value;
        float filtered_value[2];
        arm_matrix_instance_f32 xhat, xhatminus, z, A, H, AT, HT, Q, R, P, Pminus, K, E;
    } kalman;
} kalman_filterII_t;

void kalmanInitII(kalman_filterII_t *I);

float *KalmanFilterII(kalman_filterII_t *I, float signal1, float signal2);

#endif
