#ifndef BUTTERWORTHFILTER_BUTTERWORTHFILTERIIR_H
#define BUTTERWORTHFILTER_BUTTERWORTHFILTERIIR_H

#include "stdint.h"

/**
 * @brief IIR滤波器结构体
 */
typedef struct {
  float (*a)[2];
  float (*b)[2];
  float (*w)[2];
  float *s;
  float OutputGain;
  uint32_t Sections;
} IIRFilter_t;

void IIRFilterInit(IIRFilter_t *filter, uint32_t Sections,
                              const float (*Matlab_NUM)[3],
                              const float (*Matlab_DEN)[3]);
void IIRFilterDelete(IIRFilter_t *filter);
float IIRFilter(IIRFilter_t *filter, float input);

#endif //BUTTERWORTHFILTER_BUTTERWORTHFILTERIIR_H
