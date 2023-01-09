#include "LED_drv.h"


GPIO_hal_Type *LED1,*LED2,*LED3,*LED4;
uint8_t LED_Bit_State = 0x00;//LED为软件控制时状态
uint8_t Running_Water_Count = 0;
LED_State LED_Now_State = Always_extinguish;//默认常灭
Time LED_RUN_T;
void LED_drv_Init(void)
{
	LED1 = Get_GPIO_Control(4);
	LED2 = Get_GPIO_Control(5);
	LED3 = Get_GPIO_Control(6);
	LED4 = Get_GPIO_Control(7);
	LED_RUN_T = Get_System_T();
	LED1->GPIO_HIGH(LED1);
	LED2->GPIO_HIGH(LED2);
	LED3->GPIO_HIGH(LED3);
	LED4->GPIO_HIGH(LED4);
}

void Set_LED_State(LED_State state) 
{
	LED_Now_State = state;
}

void LED_RUN(void)
{
	switch(LED_Now_State)
	{
		case Always_extinguish: //灯常灭
		{
			LED1->GPIO_HIGH(LED1);
			LED2->GPIO_HIGH(LED2);
			LED3->GPIO_HIGH(LED3);
			LED4->GPIO_HIGH(LED4);
		}
		break;
		case Always_Light://灯常亮
		{
      LED1->GPIO_LOW(LED1);
			LED2->GPIO_LOW(LED2);
			LED3->GPIO_LOW(LED3);
			LED4->GPIO_LOW(LED4);
		}
		break;
		case Slow_Flicker://快闪
		{
			if(Get_deta_T(LED_RUN_T)>0.1f)//设定闪烁时间
			{
				LED1->GPIO_TOGGLE(LED1);// 高低电平切换
				LED2->GPIO_TOGGLE(LED2);
				LED3->GPIO_TOGGLE(LED3);
				LED4->GPIO_TOGGLE(LED4);
				LED_RUN_T = Get_System_T();
			}
		}
		break;
		case Fast_Flicker://慢闪
		{
			if(Get_deta_T(LED_RUN_T)>0.5f)
			{
				LED1->GPIO_TOGGLE(LED1);// 高低电平切换
				LED2->GPIO_TOGGLE(LED2);
				LED3->GPIO_TOGGLE(LED3);
				LED4->GPIO_TOGGLE(LED4);
				LED_RUN_T = Get_System_T();
			}
		}
		break;
		case Running_Water:
			if(Get_deta_T(LED_RUN_T)>0.1f)
			{
				LED_RUN_T = Get_System_T();
				switch(Running_Water_Count++)
				{
					case 4:
						Running_Water_Count=0;
					case 0:
						LED4->GPIO_HIGH(LED4);
						LED1->GPIO_LOW(LED1);
					break;
					case 1:
						LED1->GPIO_HIGH(LED1);
						LED2->GPIO_LOW(LED2);
					break;
					case 2:
						LED2->GPIO_HIGH(LED2);
						LED3->GPIO_LOW(LED3);
					break;
					case 3:
						LED3->GPIO_HIGH(LED3);
						LED4->GPIO_LOW(LED4);
					break;
				}
			}
		break;
		case State_Control:
		{
			if((LED_Bit_State&0x01) == 0x01)
				LED1->GPIO_HIGH(LED1);
			else
				LED1->GPIO_LOW(LED1);
			
			if((LED_Bit_State&0x02) == 0x02)
				LED2->GPIO_HIGH(LED2);
			else
				LED2->GPIO_LOW(LED2);
			
			if((LED_Bit_State&0x04) == 0x04)
				LED3->GPIO_HIGH(LED3);
			else
				LED3->GPIO_LOW(LED3);
			
			if((LED_Bit_State&0x08) == 0x08)
				LED4->GPIO_HIGH(LED4);
			else
				LED4->GPIO_LOW(LED4);
		}
		break;
	}
}



bool Set_LED_Bit_State(uint8_t LEDx,FlagStatus State)
{
	if(LED_Now_State == State_Control)//判断LED运行模式
	{
		switch(LEDx)
		{
			case 0:
				if(State)
					LED_Bit_State |= 0x01;
				else
					LED_Bit_State &= 0xFE;
			break;
			case 1:
				if(State)
					LED_Bit_State |= 0x02;
				else
					LED_Bit_State &= 0xFD;
			break;
			case 2:
				if(State)
					LED_Bit_State |= 0x04;
				else
					LED_Bit_State &= 0xFB;
			break;
			case 3:
				if(State)
					LED_Bit_State |= 0x08;
				else
					LED_Bit_State &= 0xF7;
			break;
		}
		return true;
	}else
	{
		return false;
	}
}






