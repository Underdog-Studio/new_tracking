#ifndef __Configuration_hal_H
#define __Configuration_hal_H

#include "stdbool.h"

#include "System_time.h"

#include "gd32e23x.h"
#include "gpio_hal.h"
#include "adc_hal.h"
#include "EXTI_hal.h"
#include "flash_hal.h"
#include "I2C_hal.h"

#define GPIO_Hal 1
#define ADC_hal 1
#define EXTI_hal 1
#define Flash_hal 1
#define I2C_hal 1

void Init_hal(void);
#endif

