#include "gd32e23x.h"
#include "System_time.h"

#include "Configuration_hal.h"

#include "Infrared_Probe_drv.h"
#include "Button_drv.h"
#include "LED_drv.h"
#include "EEPROM_drv.h"

#include "Software_Register.h"




void GPIO_AHIGH(void *dat11a);

void SoftReset(void)
{ 
  NVIC_SystemReset(); // ¸´Î»
}


int main(void)
{
	GPIO_hal_Type *LED001,*LED002;
	float pos = 0;
	uint16_t i;
	uint16_t temp[5];
	System_time_init();
	Init_hal();
	Infrared_Probe_init_drv();
	Button_init();
	LED_drv_Init();
	EEPROM_drv_Init();
	
	LED001 = Get_GPIO_Control(6);
	LED002 = Get_GPIO_Control(7);
	while(1)
	{
		Infrared_Probe();
				
		if(Get_Button_Long_Press() == true)
		{
			Infrared_Calibration();
			SoftReset();
		}
		LED_RUN();
	}
	
}



