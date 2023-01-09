#include "EXTI_hal.h"


uint8_t Get_EXTI_SOURCE(uint32_t gpio_periph);
static void (*EXTI0_Callback)(void);//外部中断0回调函数指针
static void (*EXTI1_Callback)(void);//外部中断1回调函数指针
static void (*EXTI2_Callback)(void);//外部中断2回调函数指针
static void (*EXTI3_Callback)(void);//外部中断3回调函数指针
static void (*EXTI4_Callback)(void);//外部中断4回调函数指针
static void (*EXTI5_Callback)(void);//外部中断5回调函数指针
static void (*EXTI6_Callback)(void);//外部中断6回调函数指针
static void (*EXTI7_Callback)(void);//外部中断7回调函数指针
static void (*EXTI8_Callback)(void);//外部中断8回调函数指针
static void (*EXTI9_Callback)(void);//外部中断9回调函数指针
static void (*EXTI10_Callback)(void);//外部中断10回调函数指针
static void (*EXTI11_Callback)(void);//外部中断11回调函数指针
static void (*EXTI12_Callback)(void);//外部中断12回调函数指针
static void (*EXTI13_Callback)(void);//外部中断13回调函数指针
static void (*EXTI14_Callback)(void);//外部中断14回调函数指针
static void (*EXTI15_Callback)(void);//外部中断15回调函数指针




/**
*@Description :外部中断初始化
**/
void EXTI_hal_init(void)
{
	rcu_periph_clock_enable(RCU_CFGCMP);
	#if EXTI_Lin0_hal
		EXTI0_Callback = 0;
		gpio_mode_set(GPIO_Lin0_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_0);
    syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin0_Periph),EXTI_SOURCE_PIN0);
		exti_init(EXTI_0,EXTI_Lin0_Mode,EXTI_Lin0_Trig);
	#endif
	#if EXTI_Lin1_hal
		EXTI1_Callback = 0;
		gpio_mode_set(GPIO_Lin1_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_1);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin1_Periph),EXTI_SOURCE_PIN1);
		exti_init(EXTI_1,EXTI_Lin1_Mode,EXTI_Lin1_Trig);
	#endif
	#if EXTI_Lin2_hal
		EXTI2_Callback = 0;
		gpio_mode_set(GPIO_Lin2_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_2);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin2_Periph),EXTI_SOURCE_PIN2);
		exti_init(EXTI_2,EXTI_Lin2_Mode,EXTI_Lin2_Trig);
	#endif
	#if EXTI_Lin3_hal
		EXTI3_Callback = 0;
		gpio_mode_set(GPIO_Lin3_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_3);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin3_Periph),EXTI_SOURCE_PIN3);
		exti_init(EXTI_3,EXTI_Lin3_Mode,EXTI_Lin3_Trig);
	#endif
	#if EXTI_Lin4_hal
		EXTI4_Callback = 0;
		gpio_mode_set(GPIO_Lin4_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_4);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin4_Periph),EXTI_SOURCE_PIN4);
		exti_init(EXTI_4,EXTI_Lin4_Mode,EXTI_Lin4_Trig);
	#endif
	#if EXTI_Lin5_hal
		EXTI5_Callback = 0;
		gpio_mode_set(GPIO_Lin5_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_5);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin5_Periph),EXTI_SOURCE_PIN5);
		exti_init(EXTI_5,EXTI_Lin5_Mode,EXTI_Lin5_Trig);
	#endif
	#if EXTI_Lin6_hal
		EXTI6_Callback = 0;
		gpio_mode_set(GPIO_Lin6_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_6);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin6_Periph),EXTI_SOURCE_PIN6);
		exti_init(EXTI_6,EXTI_Lin6_Mode,EXTI_Lin6_Trig);
	#endif
	#if EXTI_Lin7_hal
		EXTI7_Callback = 0;
		gpio_mode_set(GPIO_Lin7_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_7);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin7_Periph),EXTI_SOURCE_PIN7);
		exti_init(EXTI_7,EXTI_Lin7_Mode,EXTI_Lin7_Trig);
	#endif
	#if EXTI_Lin8_hal
		EXTI8_Callback = 0;
		gpio_mode_set(GPIO_Lin8_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_8);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin8_Periph),EXTI_SOURCE_PIN8);
		exti_init(EXTI_8,EXTI_Lin8_Mode,EXTI_Lin8_Trig);
	#endif
	#if EXTI_Lin9_hal
		EXTI9_Callback = 0;
		gpio_mode_set(GPIO_Lin9_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_9);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin9_Periph),EXTI_SOURCE_PIN9);
		exti_init(EXTI_9,EXTI_Lin9_Mode,EXTI_Lin9_Trig);
	#endif
	#if EXTI_Lin10_hal
		EXTI10_Callback = 0;
		gpio_mode_set(GPIO_Lin10_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_10);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin10_Periph),EXTI_SOURCE_PIN10);
		exti_init(EXTI_10,EXTI_Lin10_Mode,EXTI_Lin10_Trig);
	#endif
	#if EXTI_Lin11_hal
		EXTI11_Callback = 0;
		gpio_mode_set(GPIO_Lin11_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_11);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin11_Periph),EXTI_SOURCE_PIN11);
		exti_init(EXTI_11,EXTI_Lin11_Mode,EXTI_Lin11_Trig);
	#endif
	#if EXTI_Lin12_hal
		EXTI12_Callback = 0;
		gpio_mode_set(GPIO_Lin12_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_12);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin12_Periph),EXTI_SOURCE_PIN12);
		exti_init(EXTI_12,EXTI_Lin12_Mode,EXTI_Lin12_Trig);
	#endif
	#if EXTI_Lin13_hal
		EXTI13_Callback = 0;
		gpio_mode_set(GPIO_Lin13_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_13);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin13_Periph),EXTI_SOURCE_PIN13);
		exti_init(EXTI_13,EXTI_Lin13_Mode,EXTI_Lin13_Trig);
	#endif
	#if EXTI_Lin14_hal
		EXTI14_Callback = 0;
		gpio_mode_set(GPIO_Lin14_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_14);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin14_Periph),EXTI_SOURCE_PIN14);
		exti_init(EXTI_14,EXTI_Lin14_Mode,EXTI_Lin14_Trig);
	#endif
	#if EXTI_Lin15_hal
		EXTI15_Callback = 0;
		gpio_mode_set(GPIO_Lin15_Periph,GPIO_MODE_INPUT,GPIO_PUPD_NONE,GPIO_PIN_15);
		syscfg_exti_line_config(Get_EXTI_SOURCE(GPIO_Lin15_Periph),EXTI_SOURCE_PIN15);
		exti_init(EXTI_15,EXTI_Lin15_Mode,EXTI_Lin15_Trig);
	#endif

	
	
	#if EXTI_Lin0_hal|EXTI_Lin1_hal
		nvic_irq_enable(EXTI0_1_IRQn,2);
	  exti_interrupt_flag_clear(EXTI_0|EXTI_1);
	#endif
	#if EXTI_Lin2_hal|EXTI_Lin3_hal
		nvic_irq_enable(EXTI2_3_IRQn,0);
		exti_interrupt_flag_clear(EXTI_2|EXTI_3);
	#endif
	#if (EXTI_Lin4_hal|EXTI_Lin5_hal|EXTI_Lin6_hal|EXTI_Lin7_hal|\
	     EXTI_Lin8_hal|EXTI_Lin9_hal|EXTI_Lin10_hal|EXTI_Lin11_hal|\
	     EXTI_Lin12_hal|EXTI_Lin13_hal|EXTI_Lin14_hal|EXTI_Lin15_hal)
		nvic_irq_enable(EXTI4_15_IRQn,0);
		exti_interrupt_flag_clear(EXTI_4|EXTI_5|EXTI_6|EXTI_7|EXTI_8|EXTI_9|EXTI_10|EXTI_11|EXTI_12|EXTI_13|EXTI_14|EXTI_15);
	#endif
}




/**
*@Description :外部中断回调注册
*@param       :回调指针
**/
#if EXTI_Lin0_hal
void EXTI0_Callback_Register(void(*app)(void))
{
	EXTI0_Callback=app;
}
#endif
#if EXTI_Lin1_hal
void EXTI1_Callback_Register(void(*app)(void))
{
	EXTI1_Callback=app;
}
#endif
#if EXTI_Lin2_hal
void EXTI2_Callback_Register(void(*app)(void))
{
	EXTI2_Callback=app;
}
#endif
#if EXTI_Lin3_hal
void EXTI3_Callback_Register(void(*app)(void))
{
	EXTI3_Callback=app;
}
#endif
#if EXTI_Lin4_hal
void EXTI4_Callback_Register(void(*app)(void))
{
	EXTI4_Callback=app;
}
#endif
#if EXTI_Lin5_hal
void EXTI5_Callback_Register(void(*app)(void))
{
	EXTI5_Callback=app;
}
#endif
#if EXTI_Lin6_hal
void EXTI6_Callback_Register(void(*app)(void))
{
	EXTI6_Callback=app;
}
#endif
#if EXTI_Lin7_hal
void EXTI7_Callback_Register(void(*app)(void))
{
	EXTI7_Callback=app;
}
#endif
#if EXTI_Lin8_hal
void EXTI8_Callback_Register(void(*app)(void))
{
	EXTI8_Callback=app;
}
#endif
#if EXTI_Lin9_hal
void EXTI9_Callback_Register(void(*app)(void))
{
	EXTI9_Callback=app;
}
#endif
#if EXTI_Lin10_hal
void EXTI10_Callback_Register(void(*app)(void))
{
	EXTI10_Callback=app;
}
#endif
#if EXTI_Lin11_hal
void EXTI11_Callback_Register(void(*app)(void))
{
	EXTI11_Callback=app;
}
#endif
#if EXTI_Lin12_hal
void EXTI12_Callback_Register(void(*app)(void))
{
	EXTI12_Callback=app;
}
#endif
#if EXTI_Lin13_hal
void EXTI13_Callback_Register(void(*app)(void))
{
	EXTI13_Callback=app;
}
#endif
#if EXTI_Lin14_hal
void EXTI14_Callback_Register(void(*app)(void))
{
	EXTI14_Callback=app;
}
#endif
#if EXTI_Lin15_hal
void EXTI15_Callback_Register(void(*app)(void))
{
	EXTI15_Callback=app;
}
#endif

/**
*@Description :外部中断触发模式设置
*@param       :中断线
*@param       :中断触发模式 0:EXTI_TRIG_RISING 1:EXTI_TRIG_FALLING 2:EXTI_TRIG_BOTH
**/
void Set_EXTI_TRIG(uint8_t linex,uint8_t trig_type)
{
	switch(trig_type){
    case 0://EXTI_TRIG_RISING  上升
        EXTI_RTEN |= (uint32_t)BIT(linex);
        EXTI_FTEN &= ~(uint32_t)BIT(linex);
        break;
    case 1://EXTI_TRIG_FALLING 下降
        EXTI_RTEN &= ~(uint32_t)BIT(linex);
        EXTI_FTEN |= (uint32_t)BIT(linex);
        break;
    case 2://EXTI_TRIG_BOTH    双边沿
        EXTI_RTEN |= (uint32_t)BIT(linex);
        EXTI_FTEN |= (uint32_t)BIT(linex);
        break;
    default:
        break;
    }
}

/**
*@Description :外部中断服务函数
**/
#if EXTI_Lin0_hal|EXTI_Lin1_hal
	void EXTI0_1_IRQHandler(void)
	{
		#if EXTI_Lin0_hal
		if((RESET != exti_interrupt_flag_get(EXTI_0))&&EXTI0_Callback!=0)
		{
			EXTI0_Callback();
		}
		exti_interrupt_flag_clear(EXTI_0);
		#endif
		#if EXTI_Lin1_hal
		if((RESET != exti_interrupt_flag_get(EXTI_1))&&EXTI1_Callback!=0)
		{
			EXTI1_Callback();
		}
		exti_interrupt_flag_clear(EXTI_1);
		#endif
		
	}
#endif
#if EXTI_Lin2_hal|EXTI_Lin3_hal
	void EXTI2_3_IRQHandler(void)
	{
		#if EXTI_Lin2_hal
		if((RESET != exti_interrupt_flag_get(EXTI_2))&&EXTI2_Callback!=0)
		{
			EXTI2_Callback();
		}
	  #endif
		#if EXTI_Lin3_hal
		if((RESET != exti_interrupt_flag_get(EXTI_3))&&EXTI3_Callback!=0)
		{
			EXTI3_Callback();
		}
	  #endif
	}
#endif
#if (EXTI_Lin4_hal|EXTI_Lin5_hal|EXTI_Lin6_hal|EXTI_Lin7_hal|\
     EXTI_Lin8_hal|EXTI_Lin9_hal|EXTI_Lin10_hal|EXTI_Lin11_hal|\
     EXTI_Lin12_hal|EXTI_Lin13_hal|EXTI_Lin14_hal|EXTI_Lin15_hal)
	void EXTI4_15_IRQHandler(void)
	{
		#if EXTI_Lin4_hal
		if((RESET != exti_interrupt_flag_get(EXTI_4))&&EXTI4_Callback!=0)
		{
			EXTI4_Callback();
		}
		#endif
		#if EXTI_Lin5_hal
		if((RESET != exti_interrupt_flag_get(EXTI_5))&&EXTI5_Callback!=0)
		{
			EXTI5_Callback();
		}
		#endif
		#if EXTI_Lin6_hal
		if((RESET != exti_interrupt_flag_get(EXTI_6))&&EXTI6_Callback!=0)
		{
			EXTI6_Callback();
		}
		#endif
		#if EXTI_Lin7_hal
		if((RESET != exti_interrupt_flag_get(EXTI_7))&&EXTI7_Callback!=0)
		{
			EXTI7_Callback();
		}
		#endif
		#if EXTI_Lin8_hal
		if((RESET != exti_interrupt_flag_get(EXTI_8))&&EXTI8_Callback!=0)
		{
			EXTI8_Callback();
		}
		#endif
		#if EXTI_Lin9_hal
		if((RESET != exti_interrupt_flag_get(EXTI_9))&&EXTI9_Callback!=0)
		{
			EXTI9_Callback();
		}
		#endif
		#if EXTI_Lin10_hal
		if((RESET != exti_interrupt_flag_get(EXTI_10))&&EXTI10_Callback!=0)
		{
			EXTI10_Callback();
		}
		#endif
		#if EXTI_Lin11_hal
		if((RESET != exti_interrupt_flag_get(EXTI_11))&&EXTI11_Callback!=0)
		{
			EXTI11_Callback();
		}
		#endif
		#if EXTI_Lin12_hal
		if((RESET != exti_interrupt_flag_get(EXTI_12))&&EXTI12_Callback!=0)
		{
			EXTI12_Callback();
		}
		#endif
		#if EXTI_Lin13_hal
		if((RESET != exti_interrupt_flag_get(EXTI_13))&&EXTI13_Callback!=0)
		{
			EXTI13_Callback();
		}
		#endif
		#if EXTI_Lin14_hal
		if((RESET != exti_interrupt_flag_get(EXTI_14))&&EXTI14_Callback!=0)
		{
			EXTI14_Callback();
		}
		#endif
		#if EXTI_Lin15_hal
		if((RESET != exti_interrupt_flag_get(EXTI_15))&&EXTI15_Callback!=0)
		{
			EXTI15_Callback();
		}
		#endif
	}
#endif


/**
*@Description :获取中断端口
**/
uint8_t Get_EXTI_SOURCE(uint32_t gpio_periph)
{
	switch(gpio_periph)
	{
		case GPIOA:
			return EXTI_SOURCE_GPIOA;
		case GPIOB:
			return EXTI_SOURCE_GPIOB;
		case GPIOC:
			return EXTI_SOURCE_GPIOC;
		case GPIOF:
		  return EXTI_SOURCE_GPIOF;
	}
	return 0;
}




