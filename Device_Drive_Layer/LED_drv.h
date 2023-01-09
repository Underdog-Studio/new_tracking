#ifndef __LED_drv_H
#define __LED_drv_H

#include "Configuration_hal.h"

typedef enum
{
 Always_extinguish,//����
 Always_Light,//����
 Slow_Flicker,//����
 Fast_Flicker,//����
 State_Control,//״̬����
 Running_Water//��ˮ��
}LED_State;//LED״̬ö��
void LED_drv_Init(void);
void Set_LED_State(LED_State state);
void LED_RUN(void);
bool Set_LED_Bit_State(uint8_t LEDx,FlagStatus State);
#endif

