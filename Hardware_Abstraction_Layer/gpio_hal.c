#include "gpio_hal.h"

#define GPIO_Pin_num 8 //GPIO数量

void GPIO_HIGH_hal(void *gpio);
void GPIO_LOW_hal(void *gpio);
void GPIO_TOGGLE_hal(void *gpio);
void GPIO_Pin_Init_hal(GPIO_hal_Type *gpio);


GPIO_Base_Type GPIO_hal_Config[GPIO_Pin_num] = {
{GPIOB,    GPIO_MODE_OUTPUT,		GPIO_PIN_5},//GPIO0
{GPIOF,    GPIO_MODE_OUTPUT,		GPIO_PIN_0},//GPIO1
{GPIOB,    GPIO_MODE_OUTPUT,		GPIO_PIN_3},//GPIO2
{GPIOA,    GPIO_MODE_OUTPUT,		GPIO_PIN_8},//GPIO3
{GPIOA,    GPIO_MODE_OUTPUT,		GPIO_PIN_5},//GPIO4
{GPIOA,		 GPIO_MODE_OUTPUT,		GPIO_PIN_6},//GPIO5
{GPIOA,		 GPIO_MODE_OUTPUT,		GPIO_PIN_7},//GPIO6
{GPIOB,		 GPIO_MODE_OUTPUT,		GPIO_PIN_0},//GPIO7
};


GPIO_hal_Type GPIO_hal[GPIO_Pin_num];
/**
*@Descritpion :注册所有GPIO并初始化
**/
void GPIO_Init_hal(void)
{
	uint8_t i;
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);

	rcu_periph_clock_enable(RCU_GPIOF);
	for(i=0;i<GPIO_Pin_num;i++)
	{
		GPIO_hal[i].GPIO_HIGH = GPIO_HIGH_hal;
		GPIO_hal[i].GPIO_LOW = GPIO_LOW_hal;
		GPIO_hal[i].GPIO_TOGGLE = GPIO_TOGGLE_hal;
		GPIO_hal[i].GPIO_Base = GPIO_hal_Config[i];
		GPIO_Pin_Init_hal(&GPIO_hal[i]);
		GPIO_hal[i].GPIO_LOW(&GPIO_hal[i]);
	}

}

/**
*@Description :GPIO拉高
*@param       :GPIO控制指针
**/
void GPIO_HIGH_hal(void *gpio)
{
	GPIO_BOP((*(GPIO_hal_Type*)gpio).GPIO_Base.GPIO_Port) = (*(GPIO_hal_Type*)gpio).GPIO_Base.GPIO_PIN;
}
/**
*@Description :GPIO拉低
*@param       :GPIO控制指针
**/
void GPIO_LOW_hal(void *gpio)
{
	GPIO_BC((*(GPIO_hal_Type*)gpio).GPIO_Base.GPIO_Port) = (*(GPIO_hal_Type*)gpio).GPIO_Base.GPIO_PIN;
}
/**
*@Description :GPIO翻转
*@param       :GPIO控制指针
**/
void GPIO_TOGGLE_hal(void *gpio)
{
	GPIO_TG((*(GPIO_hal_Type*)gpio).GPIO_Base.GPIO_Port) = (*(GPIO_hal_Type*)gpio).GPIO_Base.GPIO_PIN;
}

/**
*@Description :初始化GPIO
*@param       :GPIO控制指针
**/
void GPIO_Pin_Init_hal(GPIO_hal_Type *gpio)
{
	gpio_mode_set(gpio->GPIO_Base.GPIO_Port,gpio->GPIO_Base.GPIO_Mode,GPIO_PUPD_NONE,gpio->GPIO_Base.GPIO_PIN);//默认输出模式
	gpio_output_options_set(gpio->GPIO_Base.GPIO_Port,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,gpio->GPIO_Base.GPIO_PIN);//推挽
}

GPIO_hal_Type* Get_GPIO_Control(uint8_t num)
{
	return &(GPIO_hal[num]);
}



