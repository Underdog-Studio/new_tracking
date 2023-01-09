#ifndef __adc_hal_H
#define __adc_hal_H

#include "gd32e23x_adc.h"

void adc_init(void);
uint16_t* get_value(uint8_t num);

#endif
