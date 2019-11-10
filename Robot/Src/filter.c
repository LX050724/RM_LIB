#include "stm32f4xx_hal.h"
#include "filter.h"

#ifdef ARM_MATH_CM4

#include "arm_math.h"
//kalman_filter_init_t yaw_kalman_filter_data = {
//    .P_data = {2, 0, 0, 2},     // 协方差矩阵
//    .A_data = {1, 0.001, 0, 1}, // 预测矩阵 （采样时间）
//    .H_data = {2, 0, 0, 1},     // 传感器测量数据矩阵
//    .Q_data = {0.1, 0, 0, 0.1}, // 外部的不确定性（不确定性）
//    .R_data = {0.1, 0.1, 0.1, 0.1}, // 传感器测量方差（采集数据方差）
//    .E_data = {1, 0, 0, 1}      // 单位矩阵

//		.P_data = {1,1,1,1},           // 协方差矩阵
//		.A_data = {1,0.001,0,1},        // 预测矩阵 （采样时间）
//		.H_data = {1, 0, 0, 1}, 					// 传感器测量数据矩阵
//		.Q_data = {0.01,0,0,0.01},           // 外部的不确定性（不确定性）
//		.R_data = {0.2879,0.0213,0.0213,1.8787},      // 传感器测量方差（采集数据方差）
//		.E_data = {1,0,0,1}            // 单位矩阵
//};

void kalman_filter_init(kalman_filter_init_t *I)
{
  mat_init(&I->kalman.xhat, 2, 1, (float *)I->xhat_data); //  前行后列
  mat_init(&I->kalman.xhatminus, 2, 1, (float *)I->xhatminus_data);
  mat_init(&I->kalman.z, 2, 1, (float *)I->z_data);
  mat_init(&I->kalman.A, 2, 2, (float *)I->A_data);
  mat_init(&I->kalman.H, 2, 2, (float *)I->H_data);
  mat_init(&I->kalman.AT, 2, 2, (float *)I->AT_data);
  mat_trans(&I->kalman.A, &I->kalman.AT);
  mat_init(&I->kalman.Q, 2, 2, (float *)I->Q_data);
  mat_init(&I->kalman.R, 2, 2, (float *)I->R_data);
  mat_init(&I->kalman.P, 2, 2, (float *)I->P_data);
  mat_init(&I->kalman.Pminus, 2, 2, (float *)I->Pminus_data);
  mat_init(&I->kalman.K, 2, 2, (float *)I->K_data);
  mat_init(&I->kalman.HT, 2, 2, (float *)I->HT_data);
  mat_trans(&I->kalman.H, &I->kalman.HT);
  mat_init(&I->kalman.E, 2, 2, (float *)I->E_data);
}

void kalman_filter_calc(kalman_filter_init_t *I, float signal1, float signal2)
{
	kalman_filter_t *F = &I->kalman;
	
  float TEMP_data[4] = {0, 0, 0, 0};
  float TEMP_data21[2] = {0, 0};
  mat TEMP, TEMP21;

  mat_init(&TEMP, 2, 2, (float *)TEMP_data);
  mat_init(&TEMP21, 2, 1, (float *)TEMP_data21);

  F->z.pData[0] = signal1; // 传感器读数位置值
  F->z.pData[1] = signal2; // 传感器读数速度值

  //1. xhat'(k)= A xhat(k-1)
  mat_mult(&F->A, &F->xhat, &F->xhatminus); //根据k-1时刻的值预测k时刻的值

  //2. P'(k) = A P(k-1) AT + Q
  mat_mult(&F->A, &F->P, &F->Pminus); //根据k-1时刻的方差预测k时刻的方差
  mat_mult(&F->Pminus, &F->AT, &TEMP);
  mat_add(&TEMP, &F->Q, &F->Pminus);

  //3. K(k) = P'(k) HT / (H P'(k) HT + R)     //计算卡尔曼增益
  mat_mult(&F->H, &F->Pminus, &F->K); //计算除数
  mat_mult(&F->K, &F->HT, &TEMP);
  mat_add(&TEMP, &F->R, &F->K);

  mat_inv(&F->K, &F->P); //除数转置
  mat_mult(&F->Pminus, &F->HT, &TEMP);
  mat_mult(&TEMP, &F->P, &F->K); //更新卡尔曼增益

  //4. xhat(k) = xhat'(k) + K(k) (z(k) - H xhat'(k))
  mat_mult(&F->H, &F->xhatminus, &TEMP21); //预测的均值
  mat_sub(&F->z, &TEMP21, &F->xhat);
  mat_mult(&F->K, &F->xhat, &TEMP21);
  mat_add(&F->xhatminus, &TEMP21, &F->xhat); //获得最佳估计值

  //5. P(k) = (1-K(k)H)P'(k)
  mat_mult(&F->K, &F->H, &F->P); //更新总体协方差
  mat_sub(&F->E, &F->P, &TEMP);
  mat_mult(&TEMP, &F->Pminus, &F->P);

  F->filtered_value[0] = F->xhat.pData[0];
  F->filtered_value[1] = F->xhat.pData[1];
}

/*********解算目标速度值*********/
float speed_threshold = 10.0f; // 速度阈值
float target_speed_calc(speed_calc_data_t *S, float time, float position)
{
  S->delay_cnt++;

  if (time != S->last_time)
  {
    S->speed = (position - S->last_position) / (time - S->last_time) * 1000;
#if 1
    if ((S->speed - S->processed_speed) < -speed_threshold)
    {
      S->processed_speed = S->processed_speed - speed_threshold;
    }
    else if ((S->speed - S->processed_speed) > speed_threshold)
    {
      S->processed_speed = S->processed_speed + speed_threshold;
    }
    else
#endif
      S->processed_speed = S->speed;

    S->last_time = time;
    S->last_position = position;
    S->last_speed = S->speed;
    S->delay_cnt = 0;
  }

  if (S->delay_cnt > 200) // delay 200ms speed = 0
  {
    S->processed_speed = 0;
  }

  return S->processed_speed;
}

#endif