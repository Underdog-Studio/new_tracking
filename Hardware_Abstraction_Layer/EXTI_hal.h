#ifndef __EXTI_hal_H
#define __EXTI_hal_H
#include "gd32e23x_gpio.h"
#include "gd32e23x_exti.h"

#define EXTI_Lin0_hal 0
#define GPIO_Lin0_Periph GPIOB
#define EXTI_Lin0_Mode EXTI_INTERRUPT
#define EXTI_Lin0_Trig EXTI_TRIG_BOTH

#define EXTI_Lin1_hal 1
#define GPIO_Lin1_Periph GPIOB 
#define EXTI_Lin1_Mode EXTI_INTERRUPT
#define EXTI_Lin1_Trig EXTI_TRIG_BOTH

#define EXTI_Lin2_hal 0
#define GPIO_Lin2_Periph GPIOB
#define EXTI_Lin2_Mode EXTI_INTERRUPT
#define EXTI_Lin2_Trig EXTI_TRIG_BOTH

#define EXTI_Lin3_hal 0
#define GPIO_Lin3_Periph GPIOB 
#define EXTI_Lin3_Mode EXTI_INTERRUPT
#define EXTI_Lin3_Trig EXTI_TRIG_BOTH

#define EXTI_Lin4_hal 0
#define GPIO_Lin4_Periph GPIOB 
#define EXTI_Lin4_Mode EXTI_INTERRUPT
#define EXTI_Lin4_Trig EXTI_TRIG_BOTH

#define EXTI_Lin5_hal 0
#define GPIO_Lin5_Periph GPIOB 
#define EXTI_Lin5_Mode EXTI_INTERRUPT
#define EXTI_Lin5_Trig EXTI_TRIG_BOTH

#define EXTI_Lin6_hal 0
#define GPIO_Lin6_Periph GPIOB 
#define EXTI_Lin6_Mode EXTI_INTERRUPT
#define EXTI_Lin6_Trig EXTI_TRIG_BOTH

#define EXTI_Lin7_hal 0
#define GPIO_Lin7_Periph GPIOB
#define EXTI_Lin7_Mode EXTI_INTERRUPT
#define EXTI_Lin7_Trig EXTI_TRIG_BOTH

#define EXTI_Lin8_hal 0
#define GPIO_Lin8_Periph GPIOB 
#define EXTI_Lin8_Mode EXTI_INTERRUPT
#define EXTI_Lin8_Trig EXTI_TRIG_BOTH

#define EXTI_Lin9_hal 0
#define GPIO_Lin9_Periph GPIOB 
#define EXTI_Lin9_Mode EXTI_INTERRUPT
#define EXTI_Lin9_Trig EXTI_TRIG_BOTH

#define EXTI_Lin10_hal 0
#define GPIO_Lin10_Periph GPIOB 
#define EXTI_Lin10_Mode EXTI_INTERRUPT
#define EXTI_Lin10_Trig EXTI_TRIG_BOTH

#define EXTI_Lin11_hal 0
#define GPIO_Lin11_Periph GPIOB
#define EXTI_Lin11_Mode EXTI_INTERRUPT
#define EXTI_Lin11_Trig EXTI_TRIG_BOTH

#define EXTI_Lin12_hal 0
#define GPIO_Lin12_Periph GPIOB 
#define EXTI_Lin12_Mode EXTI_INTERRUPT
#define EXTI_Lin12_Trig EXTI_TRIG_BOTH

#define EXTI_Lin13_hal 0
#define GPIO_Lin13_Periph GPIOB 
#define EXTI_Lin13_Mode EXTI_INTERRUPT
#define EXTI_Lin13_Trig EXTI_TRIG_BOTH

#define EXTI_Lin14_hal 0
#define GPIO_Lin14_Periph GPIOB 
#define EXTI_Lin14_Mode EXTI_INTERRUPT
#define EXTI_Lin14_Trig EXTI_TRIG_BOTH

#define EXTI_Lin15_hal 0
#define GPIO_Lin15_Periph GPIOB
#define EXTI_Lin15_Mode EXTI_INTERRUPT
#define EXTI_Lin15_Trig EXTI_TRIG_BOTH


#if EXTI_Lin0_hal
void EXTI0_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin1_hal
void EXTI1_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin2_hal
void EXTI2_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin3_hal
void EXTI3_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin4_hal
void EXTI4_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin5_hal
void EXTI5_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin6_hal
void EXTI6_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin7_hal
void EXTI7_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin8_hal
void EXTI8_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin9_hal
void EXTI9_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin10_hal
void EXTI10_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin11_hal
void EXTI11_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin12_hal
void EXTI12_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin13_hal
void EXTI13_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin14_hal
void EXTI14_Callback_Register(void(*app)(void));
#endif
#if EXTI_Lin15_hal
void EXTI15_Callback_Register(void(*app)(void));
#endif
void EXTI_hal_init(void);
void Set_EXTI_TRIG(uint8_t linex,uint8_t trig_type);
#endif

