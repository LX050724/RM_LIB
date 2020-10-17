#ifndef BUTTERWORTHFILTER_BUTTERWORTHFILTERIIR_H
#define BUTTERWORTHFILTER_BUTTERWORTHFILTERIIR_H

#include "stdint.h"

/**
 * @brief 巴特沃斯滤波器结构体
 */
typedef struct {
    float (*a)[2];
    float (*b)[2];
    float (*w)[2];
    float gain;
    uint32_t Sections;
} ButterworthFilterIIR_t;

void ButterworthFilterIIRInit(ButterworthFilterIIR_t *filter, uint32_t Sections,
                              const float (*Matlab_NUM)[3],
                              const float (*Matlab_DEN)[3]);
void ButterworthFilterIIRDelete(ButterworthFilterIIR_t *filter);
float ButterworthFilterIIR(ButterworthFilterIIR_t *filter, float input);

#endif //BUTTERWORTHFILTER_BUTTERWORTHFILTERIIR_H
