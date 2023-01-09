#ifndef _System_time_H
#define _System_time_H

#include "gd32e23x.h"
#include "core_cm23.h"
typedef struct
{
	uint32_t s;
	int16_t ms;
	int16_t us;
}Time;//ʱ������

//�δ�ʱ����ʼ��
void System_time_init(void);
//��ȡϵͳʱ��
Time Get_System_T(void);
//��ȡʱ����
float Get_deta_T(Time T);
//ms��ʱ
void delay_ms(uint16_t nms);
//us��ʱ
void delay_us(uint32_t nus);


Time Get_System_T1(void);










#endif



