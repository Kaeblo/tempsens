#ifndef _adc_
#define _adc_

#define Vdd 2.954
#include "stm32f4xx.h"

uint8_t ADC_EN;
int adc_convert(void);
void adc_configure(void);

#endif
