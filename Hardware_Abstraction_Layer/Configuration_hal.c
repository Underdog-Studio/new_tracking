#include "Configuration_hal.h"

void Init_hal(void)
{
	#if GPIO_Hal
		GPIO_Init_hal();
	#endif
	#if ADC_hal
		adc_init();
	#endif
	#if EXTI_hal
		EXTI_hal_init();
	#endif
	#if I2C_hal
		I2C_hal_Init();
	#endif
}









