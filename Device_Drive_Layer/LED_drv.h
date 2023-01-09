#ifndef __LED_drv_H
#define __LED_drv_H

#include "Configuration_hal.h"

typedef enum
{
 Always_extinguish,//常灭
 Always_Light,//常亮
 Slow_Flicker,//快闪
 Fast_Flicker,//慢闪
 State_Control,//状态控制
 Running_Water//流水灯
}LED_State;//LED状态枚举
void LED_drv_Init(void);
void Set_LED_State(LED_State state);
void LED_RUN(void);
bool Set_LED_Bit_State(uint8_t LEDx,FlagStatus State);
#endif

