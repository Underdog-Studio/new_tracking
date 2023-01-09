#include "Infrared_Probe_drv.h"

static GPIO_hal_Type *Infrare_A1,*Infrare_A2,*Infrare_A3,*Infrare_A4;//��������ĸ�GPIO��
bool Infrared_Start = false;//�������� �����߿�ʼΪ�١�
static uint16_t *Infrared_ADC;

Infrared_Modes Infrared_Mode; //������ģʽ
Time Infrared_T;//������ʱ��


static uint16_t Infrared_Low_Value[4];//̽ͷ�޷����ѹ static��̬ 
static uint16_t Infrared_High_Value[4];//̽ͷ�з����ѹ
static uint16_t Infrared_LineVREF[4];//  �߲ο���ѹ
static uint16_t Infrared_BackVREF[4];//��ɫ�ο���ѹ
static uint16_t Infrared_Threshold_Value[4];//̽ͷ��ֵ��ѹ
float Infrared_Correction_Value; //Ŀ����λ���е�����ֵ

Software_Register Infrared_Register;//����Ĵ���
uint8_t Infrared_Register_Buffer[4]={5,10,11,12};//����Ĵ����ڴ�����


/***����Ĵ�������
* 0Ŀ��ƫ�� 1̽ͷ״̬ 
***/
/***EEPROM����
* 0                    2                   4                   6
*̽ͷ1��ɫ�ο���ѹ   ̽ͷ2��ɫ�ο���ѹ   ̽ͷ3��ɫ�ο���ѹ   ̽ͷ4��ɫ�ο���ѹ
*  8                  10                  12                  14
*̽ͷ1�߲ο���ѹ     ̽ͷ2�߲ο���ѹ     ̽ͷ3�߲ο���ѹ     ̽ͷ4�߲ο���ѹ
* 16                               
*Ŀ��λ���е�����    
***/

/**
*@Description :����̽ͷ��ʼ��
**/
void Infrared_Probe_init_drv(void)
{
	Infrared_Register.Register_Len = 2;
	Infrared_Register.Register = Infrared_Register_Buffer;
	Infrared_Register.Permission = Infrared_Register_Buffer+2;
	Software_Register_Init(&Infrared_Register);
	//��ȡ����ĵ�ɫ�ο���ѹ
	Infrared_BackVREF[0] = EEPROM_ReadOneHalfword(0);
	Infrared_BackVREF[1] = EEPROM_ReadOneHalfword(2);
	Infrared_BackVREF[2] = EEPROM_ReadOneHalfword(4);
	Infrared_BackVREF[3] = EEPROM_ReadOneHalfword(6);
	//��ȡ������߲ο���ѹ
	Infrared_LineVREF[0] = EEPROM_ReadOneHalfword(8);
	Infrared_LineVREF[1] = EEPROM_ReadOneHalfword(10);
	Infrared_LineVREF[2] = EEPROM_ReadOneHalfword(12);
	Infrared_LineVREF[3] = EEPROM_ReadOneHalfword(14);
	
	Infrared_Threshold_Value[0] = EEPROM_ReadOneHalfword(16);//����ֵ��ѹ
	Infrared_Threshold_Value[1] = EEPROM_ReadOneHalfword(18);
	Infrared_Threshold_Value[2] = EEPROM_ReadOneHalfword(20);
	Infrared_Threshold_Value[3] = EEPROM_ReadOneHalfword(22);
	//��ȡ����ĺ���λ������ֵ
	Infrared_Correction_Value = (int16_t)EEPROM_ReadOneHalfword(24);
	
	Infrared_T = Get_System_T();
	Infrared_ADC = get_value(0);//��ȡ�������̽ͷadc
	Infrared_Mode = Analog_Mode;//Ĭ��ģ��ģʽ
	Infrare_A1 =Get_GPIO_Control(0);//��ȡ���ⷢ��̽ͷ����GPIO
	Infrare_A2 =Get_GPIO_Control(1);
	Infrare_A3 =Get_GPIO_Control(2);
	Infrare_A4 =Get_GPIO_Control(3);
	Infrare_A1->GPIO_LOW(Infrare_A1);//�ر�̽ͷ
	Infrare_A2->GPIO_LOW(Infrare_A2);
	Infrare_A3->GPIO_LOW(Infrare_A3);
	Infrare_A4->GPIO_LOW(Infrare_A4);
	Set_LED_State(State_Control);//LEDΪ״̬����ģʽ
	I2C0_Rsgister_Register(&Infrared_Register);//������Ĵ���ע�ᵽI2C0
}

/**
*@Description :����̽ͷУ׼
**/
void Infrared_Calibration(void)
{
	uint8_t Calibration_Flag = 0,i,LED_Flag = 0;
	Time LED_T;
	uint16_t Infrared_TempVREF[4] = {0,0,0,0};
	float Infrare_adc_temp1[4]={0,0,0,0},Infrare_adc_temp2[4]={0,0,0,0};
	Set_LED_State(Running_Water);//��ˮ��ģʽ
	while(1)
	{
		LED_RUN();
		if(LED_Flag==1)
		{
			LED_Flag =0;
			Set_LED_State(Slow_Flicker);//����
			LED_T = Get_System_T();
		}
		if(Get_deta_T(LED_T)>0.6 && LED_Flag==0)
		{
			Set_LED_State(Running_Water);//����
			LED_Flag = 3;
		}
		if(Get_Button_Click())
		{
			if(Calibration_Flag == 0)//��ͼ��ɫ-------------------------------------------------
			{
				Infrare_adc_temp1[0] = 0;
				Infrare_adc_temp2[0] = 0;
				Infrare_adc_temp1[1] = 0;
				Infrare_adc_temp2[1] = 0;
				Infrare_adc_temp1[2] = 0;
				Infrare_adc_temp2[2] = 0;
				Infrare_adc_temp1[3] = 0;
				Infrare_adc_temp2[3] = 0;
				
				for(i=0;i<50;i++)//50�����������ƽ��
				{
					Infrare_A1->GPIO_LOW(Infrare_A1);//�ر�̽ͷ
					Infrare_A2->GPIO_LOW(Infrare_A2);
					Infrare_A3->GPIO_LOW(Infrare_A3);
					Infrare_A4->GPIO_LOW(Infrare_A4);
					
					delay_ms(5);
					Infrare_adc_temp1[0]+=Infrared_ADC[0];
					Infrare_adc_temp1[1]+=Infrared_ADC[1];
					Infrare_adc_temp1[2]+=Infrared_ADC[2];
					Infrare_adc_temp1[3]+=Infrared_ADC[3];
					
					Infrare_A1->GPIO_HIGH(Infrare_A1);//��̽ͷ
					Infrare_A2->GPIO_HIGH(Infrare_A2);
					Infrare_A3->GPIO_HIGH(Infrare_A3);
					Infrare_A4->GPIO_HIGH(Infrare_A4);
					
					delay_ms(5);
					Infrare_adc_temp2[0]+=Infrared_ADC[0];
					Infrare_adc_temp2[1]+=Infrared_ADC[1];
					Infrare_adc_temp2[2]+=Infrared_ADC[2];
					Infrare_adc_temp2[3]+=Infrared_ADC[3];
				}
				Infrare_adc_temp1[0]/=50.0f;
				Infrare_adc_temp1[1]/=50.0f;
				Infrare_adc_temp1[2]/=50.0f;
				Infrare_adc_temp1[3]/=50.0f;
				Infrare_adc_temp2[0]/=50.0f;
				Infrare_adc_temp2[1]/=50.0f;
				Infrare_adc_temp2[2]/=50.0f;
				Infrare_adc_temp2[3]/=50.0f;	
				//���ɫ�ο���ֵ�ѹ
				Infrared_TempVREF[0] = (uint16_t)(Infrare_adc_temp2[0] - Infrare_adc_temp1[0] + 50);
				Infrared_TempVREF[1] = (uint16_t)(Infrare_adc_temp2[1] - Infrare_adc_temp1[1] + 50);
				Infrared_TempVREF[2] = (uint16_t)(Infrare_adc_temp2[2] - Infrare_adc_temp1[2] + 50);
				Infrared_TempVREF[3] = (uint16_t)(Infrare_adc_temp2[3] - Infrare_adc_temp1[3] + 50);
				
				Infrared_BackVREF[0] = Infrared_TempVREF[0];
				Infrared_BackVREF[1] = Infrared_TempVREF[1];
				Infrared_BackVREF[2] = Infrared_TempVREF[2];
				Infrared_BackVREF[3] = Infrared_TempVREF[3];
				
				EEPROM_WriteOneHalfword( 0,Infrared_TempVREF[0]);//д��eeprom
				EEPROM_WriteOneHalfword( 2,Infrared_TempVREF[1]);
				EEPROM_WriteOneHalfword( 4,Infrared_TempVREF[2]);
				EEPROM_WriteOneHalfword( 6,Infrared_TempVREF[3]);
				Calibration_Flag++;
				LED_Flag = 1;
			}else if(Calibration_Flag == 1)//��-------------------------------------------------
			{
				Infrare_adc_temp1[0] = 0;
				Infrare_adc_temp2[0] = 0;
				Infrare_adc_temp1[1] = 0;
				Infrare_adc_temp2[1] = 0;
				Infrare_adc_temp1[2] = 0;
				Infrare_adc_temp2[2] = 0;
				Infrare_adc_temp1[3] = 0;
				Infrare_adc_temp2[3] = 0;
				
				for(i=0;i<50;i++)//50�����������ƽ��
				{
					Infrare_A1->GPIO_LOW(Infrare_A1);//�ر�̽ͷ
					Infrare_A2->GPIO_LOW(Infrare_A2);
					Infrare_A3->GPIO_LOW(Infrare_A3);
					Infrare_A4->GPIO_LOW(Infrare_A4);
					
					delay_ms(1);
					Infrare_adc_temp1[0]+=Infrared_ADC[0];
					Infrare_adc_temp1[1]+=Infrared_ADC[1];
					Infrare_adc_temp1[2]+=Infrared_ADC[2];
					Infrare_adc_temp1[3]+=Infrared_ADC[3];
					
					Infrare_A1->GPIO_HIGH(Infrare_A1);//��̽ͷ
					Infrare_A2->GPIO_HIGH(Infrare_A2);
					Infrare_A3->GPIO_HIGH(Infrare_A3);
					Infrare_A4->GPIO_HIGH(Infrare_A4);
					
					delay_ms(1);
					Infrare_adc_temp2[0]+=Infrared_ADC[0];
					Infrare_adc_temp2[1]+=Infrared_ADC[1];
					Infrare_adc_temp2[2]+=Infrared_ADC[2];
					Infrare_adc_temp2[3]+=Infrared_ADC[3];
				}
				Infrare_adc_temp1[0]/=50.0f;
				Infrare_adc_temp1[1]/=50.0f;
				Infrare_adc_temp1[2]/=50.0f;
				Infrare_adc_temp1[3]/=50.0f;
				Infrare_adc_temp2[0]/=50.0f;
				Infrare_adc_temp2[1]/=50.0f;
				Infrare_adc_temp2[2]/=50.0f;
				Infrare_adc_temp2[3]/=50.0f;	
				
				//���߲ο���ֵ�ѹ
				Infrared_TempVREF[0] = (uint16_t)((Infrare_adc_temp2[0] - Infrare_adc_temp1[0] - 50)<0?(0):(Infrare_adc_temp2[0] - Infrare_adc_temp1[0] - 50));
				Infrared_TempVREF[1] = (uint16_t)((Infrare_adc_temp2[1] - Infrare_adc_temp1[1] - 50)<0?(0):(Infrare_adc_temp2[1] - Infrare_adc_temp1[1] - 50));
				Infrared_TempVREF[2] = (uint16_t)((Infrare_adc_temp2[2] - Infrare_adc_temp1[2] - 50)<0?(0):(Infrare_adc_temp2[2] - Infrare_adc_temp1[2] - 50));
				Infrared_TempVREF[3] = (uint16_t)((Infrare_adc_temp2[3] - Infrare_adc_temp1[3] - 50)<0?(0):(Infrare_adc_temp2[3] - Infrare_adc_temp1[3] - 50));
				
				Infrared_LineVREF[0] = Infrared_TempVREF[0];
				Infrared_LineVREF[1] = Infrared_TempVREF[1];
				Infrared_LineVREF[2] = Infrared_TempVREF[2];
				Infrared_LineVREF[3] = Infrared_TempVREF[3];
				
				EEPROM_WriteOneHalfword( 8,Infrared_TempVREF[0]);//д��eeprom
				EEPROM_WriteOneHalfword(10,Infrared_TempVREF[1]);
				EEPROM_WriteOneHalfword(12,Infrared_TempVREF[2]);
				EEPROM_WriteOneHalfword(14,Infrared_TempVREF[3]);
				Calibration_Flag++;
				LED_Flag = 1;
			}
			else if(Calibration_Flag == 2)//��ֵ-------------------------------------------------
			{
				Infrare_adc_temp1[0] = 0;
				Infrare_adc_temp2[0] = 0;
				Infrare_adc_temp1[1] = 0;
				Infrare_adc_temp2[1] = 0;
				Infrare_adc_temp1[2] = 0;
				Infrare_adc_temp2[2] = 0;
				Infrare_adc_temp1[3] = 0;
				Infrare_adc_temp2[3] = 0;
				
				for(i=0;i<50;i++)//50�����������ƽ��
				{
					Infrare_A1->GPIO_LOW(Infrare_A1);//�ر�̽ͷ
					Infrare_A2->GPIO_LOW(Infrare_A2);
					Infrare_A3->GPIO_LOW(Infrare_A3);
					Infrare_A4->GPIO_LOW(Infrare_A4);
					
					delay_ms(1);
					Infrare_adc_temp1[0]+=Infrared_ADC[0];
					Infrare_adc_temp1[1]+=Infrared_ADC[1];
					Infrare_adc_temp1[2]+=Infrared_ADC[2];
					Infrare_adc_temp1[3]+=Infrared_ADC[3];
					
					Infrare_A1->GPIO_HIGH(Infrare_A1);//��̽ͷ
					Infrare_A2->GPIO_HIGH(Infrare_A2);
					Infrare_A3->GPIO_HIGH(Infrare_A3);
					Infrare_A4->GPIO_HIGH(Infrare_A4);
					
					delay_ms(1);
					Infrare_adc_temp2[0]+=Infrared_ADC[0];
					Infrare_adc_temp2[1]+=Infrared_ADC[1];
					Infrare_adc_temp2[2]+=Infrared_ADC[2];
					Infrare_adc_temp2[3]+=Infrared_ADC[3];
				}
				Infrare_adc_temp1[0]/=50.0f;
				Infrare_adc_temp1[1]/=50.0f;
				Infrare_adc_temp1[2]/=50.0f;
				Infrare_adc_temp1[3]/=50.0f;
				Infrare_adc_temp2[0]/=50.0f;
				Infrare_adc_temp2[1]/=50.0f;
				Infrare_adc_temp2[2]/=50.0f;
				Infrare_adc_temp2[3]/=50.0f;	
				
				//����ֵ�ο���ֵ�ѹ
				Infrared_TempVREF[0] = (uint16_t)(Infrare_adc_temp2[0] - Infrare_adc_temp1[0]);
				Infrared_TempVREF[1] = (uint16_t)(Infrare_adc_temp2[1] - Infrare_adc_temp1[1]);
				Infrared_TempVREF[2] = (uint16_t)(Infrare_adc_temp2[2] - Infrare_adc_temp1[2]);
				Infrared_TempVREF[3] = (uint16_t)(Infrare_adc_temp2[3] - Infrare_adc_temp1[3]);
				
				Infrared_Threshold_Value[0] = Infrared_TempVREF[0];
				Infrared_Threshold_Value[1] = Infrared_TempVREF[1];
				Infrared_Threshold_Value[2] = Infrared_TempVREF[2];
				Infrared_Threshold_Value[3] = Infrared_TempVREF[3];
				
				EEPROM_WriteOneHalfword(16,Infrared_TempVREF[0]);//д��eeprom
				EEPROM_WriteOneHalfword(18,Infrared_TempVREF[1]);
				EEPROM_WriteOneHalfword(20,Infrared_TempVREF[2]);
				EEPROM_WriteOneHalfword(22,Infrared_TempVREF[3]);
				Calibration_Flag++;
				LED_Flag = 1;
			}else if(Calibration_Flag == 3)//���е�
			{
				Infrare_adc_temp1[0] = 0;
				Infrare_adc_temp2[0] = 0;
				Infrare_adc_temp1[1] = 0;
				Infrare_adc_temp2[1] = 0;
				Infrare_adc_temp1[2] = 0;
				Infrare_adc_temp2[2] = 0;
				Infrare_adc_temp1[3] = 0;
				Infrare_adc_temp2[3] = 0;
				
				for(i=0;i<50;i++)//50�����������ƽ��
				{
					Infrare_A1->GPIO_LOW(Infrare_A1);//�ر�̽ͷ
					Infrare_A2->GPIO_LOW(Infrare_A2);
					Infrare_A3->GPIO_LOW(Infrare_A3);
					Infrare_A4->GPIO_LOW(Infrare_A4);
					
					delay_ms(1);
					Infrare_adc_temp1[0]+=Infrared_ADC[0];
					Infrare_adc_temp1[1]+=Infrared_ADC[1];
					Infrare_adc_temp1[2]+=Infrared_ADC[2];
					Infrare_adc_temp1[3]+=Infrared_ADC[3];
					
					Infrare_A1->GPIO_HIGH(Infrare_A1);//��̽ͷ
					Infrare_A2->GPIO_HIGH(Infrare_A2);
					Infrare_A3->GPIO_HIGH(Infrare_A3);
					Infrare_A4->GPIO_HIGH(Infrare_A4);
					
					delay_ms(1);
					Infrare_adc_temp2[0]+=Infrared_ADC[0];
					Infrare_adc_temp2[1]+=Infrared_ADC[1];
					Infrare_adc_temp2[2]+=Infrared_ADC[2];
					Infrare_adc_temp2[3]+=Infrared_ADC[3];
				}
				Infrare_adc_temp1[0]/=50.0f;
				Infrare_adc_temp1[1]/=50.0f;
				Infrare_adc_temp1[2]/=50.0f;
				Infrare_adc_temp1[3]/=50.0f;
				Infrare_adc_temp2[0]/=50.0f;
				Infrare_adc_temp2[1]/=50.0f;
				Infrare_adc_temp2[2]/=50.0f;
				Infrare_adc_temp2[3]/=50.0f;	
				//�����е�ƫ��
				Infrared_Correction_Value = ((Infrare_adc_temp2[1] - Infrare_adc_temp1[1]) - (Infrare_adc_temp2[2] - Infrare_adc_temp1[2]));
				EEPROM_WriteOneHalfword(24,(uint16_t)Infrared_Correction_Value);
				EEPROM_Data_Save();//��������
				Calibration_Flag++;
				Set_LED_State(State_Control);
				break;
			}
		}
	}
}

/**
*@Description :����̽ͷɨ����
**/
#if 1
void Infrared_Probe(void)
{
	static uint8_t Line_Pos = 0;
	static uint8_t Infrared_Flag=0x00;
	float temp1,temp2;
	if(Get_deta_T(Infrared_T)>0.001f)
	{
    Infrared_T = Get_System_T();
		if(Infrared_Start == true)
		{
			Infrared_Start = false;
			*(uint64_t*)Infrared_High_Value = *(uint64_t*)Infrared_ADC;//��ȡ�޺��ⷢ���µ�ѹֵ
			Infrare_A1->GPIO_LOW(Infrare_A1);
			Infrare_A2->GPIO_LOW(Infrare_A2);
			Infrare_A3->GPIO_LOW(Infrare_A3);
			Infrare_A4->GPIO_LOW(Infrare_A4);
		}else
		{
			Infrared_Start = true;
			*(uint64_t*)Infrared_Low_Value = *(uint64_t*)Infrared_ADC;//��ȡ�к��ⷢ���µ�ѹֵ
			Infrare_A1->GPIO_HIGH(Infrare_A1);
			Infrare_A2->GPIO_HIGH(Infrare_A2);
			Infrare_A3->GPIO_HIGH(Infrare_A3);
			Infrare_A4->GPIO_HIGH(Infrare_A4);
		}
		{
			temp1 = Infrared_High_Value[0]-Infrared_Low_Value[0];
			if(temp1>(Infrared_LineVREF[0]) && temp1<Infrared_BackVREF[0])//̽ͷ1��������Χ
			{
				temp1>Infrared_Threshold_Value[0]?(Infrared_Flag|=0x08,Set_LED_Bit_State(0,SET)):(Infrared_Flag&=0xF7,Set_LED_Bit_State(0,RESET));//1
			}
			temp1 = Infrared_High_Value[3]-Infrared_Low_Value[3];
			if(temp1>(Infrared_LineVREF[3]) && temp1<Infrared_BackVREF[3])//̽ͷ4��������Χ
			{
				temp1>Infrared_Threshold_Value[3]?(Infrared_Flag|=0x01,Set_LED_Bit_State(3,SET)):(Infrared_Flag&=0xFE,Set_LED_Bit_State(3,RESET));//4
			}
			temp1 = Infrared_High_Value[1]-Infrared_Low_Value[1];
			if(temp1>(Infrared_LineVREF[1]) && temp1<Infrared_BackVREF[1])//̽ͷ2��������Χ
			{
				temp1>Infrared_Threshold_Value[1]?(Infrared_Flag|=0x04,Set_LED_Bit_State(1,SET)):(Infrared_Flag&=0xFB,Set_LED_Bit_State(1,RESET));//2
			}
			temp2 = Infrared_High_Value[2]-Infrared_Low_Value[2];
			if(temp2>(Infrared_LineVREF[2]) && temp2<Infrared_BackVREF[2])//̽ͷ3��������Χ
			{
				temp2>Infrared_Threshold_Value[2]?(Infrared_Flag|=0x02,Set_LED_Bit_State(2,SET)):(Infrared_Flag&=0xFD,Set_LED_Bit_State(2,RESET));//3
			}
			Line_Pos = (uint8_t)(127+0.15f*(temp1-temp2-Infrared_Correction_Value));//���������ƫ�Ʊ���
			Software_Register_Write(&Infrared_Register,0,&Infrared_Flag);
			Software_Register_Write(&Infrared_Register,1,&Line_Pos);
		}
	}
	
}
#endif
#if 0
void Infrared_Probe(void)
{
	static int Value11[4] = {0,0,0,0};
	static int Value22[4] = {0,0,0,0};
	static int Value33[4] = {0,0,0,0};
	Infrare_A1.GPIO_LOW();//�ر�̽ͷ
	Infrare_A2.GPIO_LOW();
	Infrare_A3.GPIO_LOW();
	Infrare_A4.GPIO_LOW();
	delay_ms(1);
	Value11[0] = Infrared_ADC[0];
	Value11[1] = Infrared_ADC[1];
	Value11[2] = Infrared_ADC[2];
	Value11[3] = Infrared_ADC[3];
	
	Infrare_A1.GPIO_HIGH();//�ر�̽ͷ
	Infrare_A2.GPIO_HIGH();
	Infrare_A3.GPIO_HIGH();
	Infrare_A4.GPIO_HIGH();
	delay_ms(1);
	Value33[0] = Infrared_ADC[0];
	Value33[1] = Infrared_ADC[1];
	Value33[2] = Infrared_ADC[2];
	Value33[3] = Infrared_ADC[3];
	Value22[0] = Value33[0] - Value11[0];
	Value22[1] = Value33[1] - Value11[1];
	Value22[2] = Value33[2] - Value11[2];
	Value22[3] = Value33[3] - Value11[3];
}
#endif

