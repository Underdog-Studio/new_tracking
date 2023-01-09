#ifndef __Infrared_Probe_drv_H
#define __Infrared_Probe_drv_H
#include "Configuration_hal.h"

#include "System_time.h"
#include "Software_Register.h"
#include "EEPROM_drv.h"
#include "Button_drv.h"
#include "LED_drv.h"

#include "math.h"
typedef enum 
{
	Digital_Mode,//����ģʽ
	Analog_Mode//ģ��ģʽ
}Infrared_Modes;


void Infrared_Probe_init_drv(void);
void Infrared_Calibration(void);
void Infrared_Probe(void);



#endif



