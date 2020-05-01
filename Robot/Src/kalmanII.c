/**
 * @file    kalmanII.c
 * @author  yao
 * @date    1-May-2020
 * @brief  二阶卡尔曼滤波器模块
 * @note 此模块依赖DSP库
 * @details
 *   初始化示例
 *
 *          kalman_filter_t kalman_filter = {
 *              .P_data = {2, 0, 0, 2},                       // 协方差矩阵
 *              .A_data = {1, 0.001, 0, 1},                   // 预测矩阵 （采样时间）
 *              .H_data = {2, 0, 0, 1},                       // 传感器测量数据矩阵
 *              .Q_data = {0.1, 0, 0, 0.1},                   // 外部的不确定性（不确定性）
 *              .R_data = {0.1, 0.1, 0.1, 0.1},               // 传感器测量方差（采集数据方差）
 *              };
 */

#include "kalmanII.h"

#include "arm_math.h"

static const float E_data[] = {1, 0, 0, 1}; //!<@brief 二阶单位矩阵常量

/**
 * @brief 初始化二阶卡尔曼滤波器
 * @param[in] I 二阶卡尔曼滤波器
 */
void kalmanInitII(kalman_filterII_t *I) {
    arm_mat_init_f32(&I->kalman.xhat, 2, 1, (float *) I->xhat_data);
    arm_mat_init_f32(&I->kalman.xhatminus, 2, 1, (float *) I->xhatminus_data);
    arm_mat_init_f32(&I->kalman.z, 2, 1, (float *) I->z_data);
    arm_mat_init_f32(&I->kalman.A, 2, 2, (float *) I->A_data);
    arm_mat_init_f32(&I->kalman.H, 2, 2, (float *) I->H_data);
    arm_mat_init_f32(&I->kalman.AT, 2, 2, (float *) I->AT_data);
    arm_mat_trans_f32(&I->kalman.A, &I->kalman.AT);
    arm_mat_init_f32(&I->kalman.Q, 2, 2, (float *) I->Q_data);
    arm_mat_init_f32(&I->kalman.R, 2, 2, (float *) I->R_data);
    arm_mat_init_f32(&I->kalman.P, 2, 2, (float *) I->P_data);
    arm_mat_init_f32(&I->kalman.Pminus, 2, 2, (float *) I->Pminus_data);
    arm_mat_init_f32(&I->kalman.K, 2, 2, (float *) I->K_data);
    arm_mat_init_f32(&I->kalman.HT, 2, 2, (float *) I->HT_data);
    arm_mat_trans_f32(&I->kalman.H, &I->kalman.HT);
    arm_mat_init_f32(&I->kalman.E, 2, 2, (float *) E_data);
}

/**
 * @brief 滤波器
 * @param[in] I 二阶卡尔曼滤波器
 * @param[in] signal1 信号1
 * @param[in] signal2 信号2
 * @return 滤波后的信号float数组,长度为2
 */
float *KalmanFilterII(kalman_filterII_t *I, float signal1, float signal2) {
    struct kalman_filtercore *F = &I->kalman;

    float TEMP_data[4] = {0, 0, 0, 0};
    float TEMP_data21[2] = {0, 0};
    arm_matrix_instance_f32 TEMP, TEMP21;

    arm_mat_init_f32(&TEMP, 2, 2, (float *) TEMP_data);
    arm_mat_init_f32(&TEMP21, 2, 1, (float *) TEMP_data21);

    /* 传感器读数位置值 */
    F->z.pData[0] = signal1;

    /* 传感器读数速度值 */
    F->z.pData[1] = signal2;

    /* 1. xhat'(k)= A xhat(k-1) */

    /* 根据k-1时刻的值预测k时刻的值 */
    arm_mat_mult_f32(&F->A, &F->xhat, &F->xhatminus);

    /* 2. P'(k) = A P(k-1) AT + Q */

    /* 根据k-1时刻的方差预测k时刻的方差 */
    arm_mat_mult_f32(&F->A, &F->P, &F->Pminus);
    arm_mat_mult_f32(&F->Pminus, &F->AT, &TEMP);
    arm_mat_add_f32(&TEMP, &F->Q, &F->Pminus);

    /* 3. K(k) = P'(k) HT / (H P'(k) HT + R) 计算卡尔曼增益 */

    /*  计算除数 */
    arm_mat_mult_f32(&F->H, &F->Pminus, &F->K);
    arm_mat_mult_f32(&F->K, &F->HT, &TEMP);
    arm_mat_add_f32(&TEMP, &F->R, &F->K);

    /* 除数转置 */
    arm_mat_inverse_f32(&F->K, &F->P);
    arm_mat_mult_f32(&F->Pminus, &F->HT, &TEMP);

    /* 更新卡尔曼增益 */
    arm_mat_mult_f32(&TEMP, &F->P, &F->K);

    /* 4. xhat(k) = xhat'(k) + K(k) (z(k) - H xhat'(k)) */

    /* 预测的均值 */
    arm_mat_mult_f32(&F->H, &F->xhatminus, &TEMP21);
    arm_mat_sub_f32(&F->z, &TEMP21, &F->xhat);
    arm_mat_mult_f32(&F->K, &F->xhat, &TEMP21);

    /* 获得最佳估计值 */
    arm_mat_add_f32(&F->xhatminus, &TEMP21, &F->xhat);

    /* 5. P(k) = (1-K(k)H)P'(k) */

    /* 更新总体协方差 */
    arm_mat_mult_f32(&F->K, &F->H, &F->P);
    arm_mat_sub_f32(&F->E, &F->P, &TEMP);
    arm_mat_mult_f32(&TEMP, &F->Pminus, &F->P);

    F->filtered_value[0] = F->xhat.pData[0];
    F->filtered_value[1] = F->xhat.pData[1];
    return F->filtered_value;
}

