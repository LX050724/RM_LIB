/**
 * @file    IIRFilter.c
 * @author  Yao
 * @date    17-Oct-2020
 * @brief   任意阶IIR直接II型，二阶节滤波器模块
 * @details
 *   实现过程完全与硬件无关，可直接调用，任意移植。
 *   先由Matlab的fdatool Filter Designer工具箱设计一个IIR直接II型，二阶节巴特沃斯滤波器，然后导出C语言头文件，取得两个参数矩阵
 *   使用时先定义一个IIRFilter_t对象，然后调用IIRFilterInit将其根据传入的节数用Matlab导出的系数初始化
 *   每次读取到传感器数据后即可调用IIRFilter来对数据进行滤波。
 *   使用完成后使用IIRFilterIIRDelete将内存释放
 *   仅支持二阶节
 */

#include "IIRFilter.h"

#ifdef __USE_RTOS
#include "FreeRTOS.h"
#include "cmsis_os.h"
#define __MALLOC(SIZE) pvPortMalloc(SIZE)
#define __FREE(P) vPortFree(P)
#else
#include "stdlib.h"
#define __MALLOC(SIZE) malloc(SIZE)
#define __FREE(P) free(P)
#endif

/**
 * @brief N阶IIR滤波器初始化
 * @param filter 滤波器
 * @param Sections 滤波器节数
 * @param Matlab_NUM Matlab导出头文件NUM系数
 * @param Matlab_DEN Matlab导出头文件DEN系数
 */
void IIRFilterInit(IIRFilter_t *filter, uint32_t Sections,
                const float (*Matlab_NUM)[3],
                const float (*Matlab_DEN)[3]) {
  filter->Sections = Sections;
  filter->a = (float (*)[2]) __MALLOC(sizeof(float) * 2 * filter->Sections);
  filter->b = (float (*)[2]) __MALLOC(sizeof(float) * 2 * filter->Sections);
  filter->w = (float (*)[2]) __MALLOC(sizeof(float) * 2 * filter->Sections);
  filter->s = (float *) __MALLOC(sizeof(float) * filter->Sections);
  for (uint32_t i = 0; i < filter->Sections; i++) {
    filter->w[i][0] = filter->w[i][1] = 0.0f;
    filter->a[i][0] = Matlab_DEN[i * 2 + 1][1];
    filter->a[i][1] = Matlab_DEN[i * 2 + 1][2];
    filter->s[i] = Matlab_NUM[i * 2][0];
    filter->b[i][0] = Matlab_NUM[i * 2 + 1][1];
    filter->b[i][1] = Matlab_NUM[i * 2 + 1][2];
  }
  filter->OutputGain = Matlab_NUM[Sections * 2][0];
}

/**
 * @brief 释放IIR滤波器内存
 * @param filter IIR滤波器
 */
void IIRFilterDelete(IIRFilter_t *filter) {
  __FREE(filter->a);
  __FREE(filter->b);
  __FREE(filter->w);
  __FREE(filter->s);
  filter->a = NULL;
  filter->b = NULL;
  filter->w = NULL;
  filter->s = NULL;
}

/**
 * @brief IIR滤波器单节运算
 * @param a 系数a向量
 * @param b 系数b向量
 * @param w 历史状态向量
 * @param input 信号输入
 * @return 滤波信号输出
 */
static inline float IIRFilter1Section(float *a, float *b, float s, float *w, float input) {
  float t = input * s  - a[0] * w[0] - a[1] * w[1];
  float r = t + b[0] * w[0] + b[1] * w[1];
  w[1] = w[0];
  w[0] = t;
  return r;
}

/**
 * @brief N阶IIR滤波器
 * @param filter 滤波器
 * @param input 信号输入
 * @return 滤波信号输出
 */
float IIRFilter(IIRFilter_t *filter, float input) {
  if (filter->a == NULL || filter->b == NULL || filter->w == NULL || filter->s == NULL)
    return 0.0f;
  for (uint32_t i = 0; i < filter->Sections; i++)
    input = IIRFilter1Section(filter->a[i], filter->b[i], filter->s[i], filter->w[i], input);
  return input * filter->OutputGain;
}
