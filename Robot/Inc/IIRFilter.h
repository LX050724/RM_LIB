/**
 * @file    IIRFilter.h
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

#ifndef BUTTERWORTHFILTER_BUTTERWORTHFILTERIIR_H
#define BUTTERWORTHFILTER_BUTTERWORTHFILTERIIR_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief IIR滤波器结构体
 */
typedef struct {
    float (*Matlab_DEN)[3];
    float (*Matlab_NUM)[3];
    float (*w)[2];
    uint32_t Sections;
} IIRFilter_t;

/**
 * @brief N阶IIR滤波器初始化
 * @param filter 滤波器
 * @param Sections 滤波器节数
 * @param Matlab_NUM Matlab导出头文件NUM系数
 * @param Matlab_DEN Matlab导出头文件DEN系数
 */
void IIRFilterInit(IIRFilter_t *filter, uint32_t Sections,
                   const float (*Matlab_NUM)[3],
                   const float (*Matlab_DEN)[3]);

/**
 * @brief N阶IIR滤波器
 * @param filter 滤波器
 * @param input 信号输入
 * @return 滤波信号输出
 */
float IIRFilter(IIRFilter_t *filter, float input);

/**
 * @brief 释放IIR滤波器内存
 * @param filter IIR滤波器
 */
void IIRFilterDelete(IIRFilter_t *filter);

#ifdef __cplusplus
}
#endif

#endif //BUTTERWORTHFILTER_BUTTERWORTHFILTERIIR_H
