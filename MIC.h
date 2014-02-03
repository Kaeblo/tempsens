#ifndef _mic_h_
#define _mic_h_
#include <stm32f4xx.h>

void PDM_init(void);

uint32_t I2SReceiveData(void);

#endif
