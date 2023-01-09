/*****************************************************************
File name:�δ�ʱ��ϵͳʵʱʱ��

Author:�Ի���

Version:V0.2

Description:�ṩϵͳʱ�� �ṩ��ʱ

Others:

Log:2021/4/28
*****************************************************************/

/*******ͷ�ļ�����********/
#include "System_time.h"

/*************************/


/*****************************************************************************************
//
//ʹ��ǰ���������ݽ����޸�
//
//�δ�ʱ�����ú����޸� ���ж����ȼ��޸�Ϊ��ߣ�
//static __INLINE uint32_t SysTick_Config(uint32_t ticks)
//{ 
//  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            
//                                                               
//  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      
//   NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,0));  
//  SysTick->VAL   = 0;                                        
//  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
//                   SysTick_CTRL_TICKINT_Msk   | 
//                   SysTick_CTRL_ENABLE_Msk;                   
//  return (0);                                                  
//}
//
//
//�궨���޸�  (ʱ���޸�Ϊ72m)
//#define SysTick_CTRL_TICKINT_Pos            1                                            
//#define SysTick_CTRL_TICKINT_Msk           (1ul << SysTick_CTRL_TICKINT_Pos)              
//
//
//
//
//
*****************************************************************************************/



/*
*@ Description �δ�ʱ����ʼ����ʼ��
*@ prarm �޲���
*@ return �޷���ֵ
*/
static Time System_T;
void System_time_init(void)
{
	SysTick_Config(SystemCoreClock/1000);	
	System_T.s=0;
	System_T.ms=0;
	System_T.us=0;
}

/*
*@ Description �δ�ʱ���жϴ�����
*@ prarm �޲���
*@ return �޷���ֵ
*/
void SysTick_Handler(void)
{
		System_T.ms++;
		if(System_T.ms>=1000)
		{
			System_T.ms=0;
			System_T.s++;
		}
}

/*
*@ Description ��ȡϵͳ��ǰʱ��
*@ prarm �޲���
*@ return ϵͳ��ǰʱ��
*/
Time Get_System_T(void)
{
	uint8_t i=0;
	System_T.us=1000-((SysTick->VAL)/108);
	i++;
	return System_T;
}

Time Get_System_T1(void)
{
	uint8_t i=0;
	System_T.us=1000-((SysTick->VAL)/108);
	i++;
	return System_T;
}

/*
*@ Description ��ȡʱ���� ��λS
*@ prarm ��ʼʱ��
*@ return ʱ����
*/
float Get_deta_T(Time T)
{
	return (System_T.s-T.s)
					+(System_T.ms-T.ms)/1000.0f
					+(Get_System_T().us-T.us)/1000000.0f;
}

/*
*@ Description us��ʱ����
*@ prarm usʱ��
*@ return �޷���ֵ
*/
void delay_us(uint32_t nus)
{
	Time Last_T=Get_System_T();
	while((Get_deta_T(Last_T))<=nus/1000000.0f);
}
	
/*
*@ Description ms��ʱ����
*@ prarm msʱ��
*@ return �޷���ֵ
*/
void delay_ms(uint16_t nms)
{
 	Time Last_T=Get_System_T();
	while((Get_deta_T(Last_T))<=nms/1000.0f);
}




