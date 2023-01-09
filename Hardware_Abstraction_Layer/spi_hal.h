#ifndef __spi_hal_H
#define __spi_hal_H

#include "gd32e23x_spi.h"

void spi0_init(void);
uint8_t spi0_readwrite_byte(uint8_t data);

#endif


