#ifndef __gpio_hal_H
#define __gpio_hal_H
#include "Configuration_hal.h"




typedef struct
{
	uint32_t GPIO_Port;
	uint32_t GPIO_Mode;
	uint32_t  GPIO_PIN;
}GPIO_Base_Type;

typedef struct
{
  GPIO_Base_Type GPIO_Base;
	void(*GPIO_HIGH)(void*);
	void(*GPIO_LOW)(void*);
	void(*GPIO_TOGGLE)(void*);
}GPIO_hal_Type;




void GPIO_Init_hal(void);
GPIO_hal_Type* Get_GPIO_Control(uint8_t num);
#endif

