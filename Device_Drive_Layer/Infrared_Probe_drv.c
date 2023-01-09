#include "Infrared_Probe_drv.h"

static GPIO_hal_Type *Infrare_A1,*Infrare_A2,*Infrare_A3,*Infrare_A4;//定义红外四个GPIO口
bool Infrared_Start = false;//布尔类型 红外线开始为假·
static uint16_t *Infrared_ADC;

Infrared_Modes Infrared_Mode; //红外线模式
Time Infrared_T;//红外线时钟


static uint16_t Infrared_Low_Value[4];//探头无发射电压 static静态 
static uint16_t Infrared_High_Value[4];//探头有发射电压
static uint16_t Infrared_LineVREF[4];//  线参考电压
static uint16_t Infrared_BackVREF[4];//底色参考电压
static uint16_t Infrared_Threshold_Value[4];//探头阈值电压
float Infrared_Correction_Value; //目标线位置中点修正值

Software_Register Infrared_Register;//软件寄存器
uint8_t Infrared_Register_Buffer[4]={5,10,11,12};//软件寄存器内存数组


/***软件寄存器数据
* 0目标偏移 1探头状态 
***/
/***EEPROM数据
* 0                    2                   4                   6
*探头1底色参考电压   探头2底色参考电压   探头3底色参考电压   探头4底色参考电压
*  8                  10                  12                  14
*探头1线参考电压     探头2线参考电压     探头3线参考电压     探头4线参考电压
* 16                               
*目标位置中点修正    
***/

/**
*@Description :红外探头初始化
**/
void Infrared_Probe_init_drv(void)
{
	Infrared_Register.Register_Len = 2;
	Infrared_Register.Register = Infrared_Register_Buffer;
	Infrared_Register.Permission = Infrared_Register_Buffer+2;
	Software_Register_Init(&Infrared_Register);
	//读取保存的底色参考电压
	Infrared_BackVREF[0] = EEPROM_ReadOneHalfword(0);
	Infrared_BackVREF[1] = EEPROM_ReadOneHalfword(2);
	Infrared_BackVREF[2] = EEPROM_ReadOneHalfword(4);
	Infrared_BackVREF[3] = EEPROM_ReadOneHalfword(6);
	//读取保存的线参考电压
	Infrared_LineVREF[0] = EEPROM_ReadOneHalfword(8);
	Infrared_LineVREF[1] = EEPROM_ReadOneHalfword(10);
	Infrared_LineVREF[2] = EEPROM_ReadOneHalfword(12);
	Infrared_LineVREF[3] = EEPROM_ReadOneHalfword(14);
	
	Infrared_Threshold_Value[0] = EEPROM_ReadOneHalfword(16);//求阈值电压
	Infrared_Threshold_Value[1] = EEPROM_ReadOneHalfword(18);
	Infrared_Threshold_Value[2] = EEPROM_ReadOneHalfword(20);
	Infrared_Threshold_Value[3] = EEPROM_ReadOneHalfword(22);
	//读取保存的黑线位置修正值
	Infrared_Correction_Value = (int16_t)EEPROM_ReadOneHalfword(24);
	
	Infrared_T = Get_System_T();
	Infrared_ADC = get_value(0);//获取红外接收探头adc
	Infrared_Mode = Analog_Mode;//默认模拟模式
	Infrare_A1 =Get_GPIO_Control(0);//获取红外发射探头控制GPIO
	Infrare_A2 =Get_GPIO_Control(1);
	Infrare_A3 =Get_GPIO_Control(2);
	Infrare_A4 =Get_GPIO_Control(3);
	Infrare_A1->GPIO_LOW(Infrare_A1);//关闭探头
	Infrare_A2->GPIO_LOW(Infrare_A2);
	Infrare_A3->GPIO_LOW(Infrare_A3);
	Infrare_A4->GPIO_LOW(Infrare_A4);
	Set_LED_State(State_Control);//LED为状态控制模式
	I2C0_Rsgister_Register(&Infrared_Register);//将软件寄存器注册到I2C0
}

/**
*@Description :红外探头校准
**/
void Infrared_Calibration(void)
{
	uint8_t Calibration_Flag = 0,i,LED_Flag = 0;
	Time LED_T;
	uint16_t Infrared_TempVREF[4] = {0,0,0,0};
	float Infrare_adc_temp1[4]={0,0,0,0},Infrare_adc_temp2[4]={0,0,0,0};
	Set_LED_State(Running_Water);//流水灯模式
	while(1)
	{
		LED_RUN();
		if(LED_Flag==1)
		{
			LED_Flag =0;
			Set_LED_State(Slow_Flicker);//快闪
			LED_T = Get_System_T();
		}
		if(Get_deta_T(LED_T)>0.6 && LED_Flag==0)
		{
			Set_LED_State(Running_Water);//快闪
			LED_Flag = 3;
		}
		if(Get_Button_Click())
		{
			if(Calibration_Flag == 0)//地图底色-------------------------------------------------
			{
				Infrare_adc_temp1[0] = 0;
				Infrare_adc_temp2[0] = 0;
				Infrare_adc_temp1[1] = 0;
				Infrare_adc_temp2[1] = 0;
				Infrare_adc_temp1[2] = 0;
				Infrare_adc_temp2[2] = 0;
				Infrare_adc_temp1[3] = 0;
				Infrare_adc_temp2[3] = 0;
				
				for(i=0;i<50;i++)//50组数据求和求平均
				{
					Infrare_A1->GPIO_LOW(Infrare_A1);//关闭探头
					Infrare_A2->GPIO_LOW(Infrare_A2);
					Infrare_A3->GPIO_LOW(Infrare_A3);
					Infrare_A4->GPIO_LOW(Infrare_A4);
					
					delay_ms(5);
					Infrare_adc_temp1[0]+=Infrared_ADC[0];
					Infrare_adc_temp1[1]+=Infrared_ADC[1];
					Infrare_adc_temp1[2]+=Infrared_ADC[2];
					Infrare_adc_temp1[3]+=Infrared_ADC[3];
					
					Infrare_A1->GPIO_HIGH(Infrare_A1);//打开探头
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
				//求底色参考差分电压
				Infrared_TempVREF[0] = (uint16_t)(Infrare_adc_temp2[0] - Infrare_adc_temp1[0] + 50);
				Infrared_TempVREF[1] = (uint16_t)(Infrare_adc_temp2[1] - Infrare_adc_temp1[1] + 50);
				Infrared_TempVREF[2] = (uint16_t)(Infrare_adc_temp2[2] - Infrare_adc_temp1[2] + 50);
				Infrared_TempVREF[3] = (uint16_t)(Infrare_adc_temp2[3] - Infrare_adc_temp1[3] + 50);
				
				Infrared_BackVREF[0] = Infrared_TempVREF[0];
				Infrared_BackVREF[1] = Infrared_TempVREF[1];
				Infrared_BackVREF[2] = Infrared_TempVREF[2];
				Infrared_BackVREF[3] = Infrared_TempVREF[3];
				
				EEPROM_WriteOneHalfword( 0,Infrared_TempVREF[0]);//写入eeprom
				EEPROM_WriteOneHalfword( 2,Infrared_TempVREF[1]);
				EEPROM_WriteOneHalfword( 4,Infrared_TempVREF[2]);
				EEPROM_WriteOneHalfword( 6,Infrared_TempVREF[3]);
				Calibration_Flag++;
				LED_Flag = 1;
			}else if(Calibration_Flag == 1)//线-------------------------------------------------
			{
				Infrare_adc_temp1[0] = 0;
				Infrare_adc_temp2[0] = 0;
				Infrare_adc_temp1[1] = 0;
				Infrare_adc_temp2[1] = 0;
				Infrare_adc_temp1[2] = 0;
				Infrare_adc_temp2[2] = 0;
				Infrare_adc_temp1[3] = 0;
				Infrare_adc_temp2[3] = 0;
				
				for(i=0;i<50;i++)//50组数据求和求平均
				{
					Infrare_A1->GPIO_LOW(Infrare_A1);//关闭探头
					Infrare_A2->GPIO_LOW(Infrare_A2);
					Infrare_A3->GPIO_LOW(Infrare_A3);
					Infrare_A4->GPIO_LOW(Infrare_A4);
					
					delay_ms(1);
					Infrare_adc_temp1[0]+=Infrared_ADC[0];
					Infrare_adc_temp1[1]+=Infrared_ADC[1];
					Infrare_adc_temp1[2]+=Infrared_ADC[2];
					Infrare_adc_temp1[3]+=Infrared_ADC[3];
					
					Infrare_A1->GPIO_HIGH(Infrare_A1);//打开探头
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
				
				//求线参考差分电压
				Infrared_TempVREF[0] = (uint16_t)((Infrare_adc_temp2[0] - Infrare_adc_temp1[0] - 50)<0?(0):(Infrare_adc_temp2[0] - Infrare_adc_temp1[0] - 50));
				Infrared_TempVREF[1] = (uint16_t)((Infrare_adc_temp2[1] - Infrare_adc_temp1[1] - 50)<0?(0):(Infrare_adc_temp2[1] - Infrare_adc_temp1[1] - 50));
				Infrared_TempVREF[2] = (uint16_t)((Infrare_adc_temp2[2] - Infrare_adc_temp1[2] - 50)<0?(0):(Infrare_adc_temp2[2] - Infrare_adc_temp1[2] - 50));
				Infrared_TempVREF[3] = (uint16_t)((Infrare_adc_temp2[3] - Infrare_adc_temp1[3] - 50)<0?(0):(Infrare_adc_temp2[3] - Infrare_adc_temp1[3] - 50));
				
				Infrared_LineVREF[0] = Infrared_TempVREF[0];
				Infrared_LineVREF[1] = Infrared_TempVREF[1];
				Infrared_LineVREF[2] = Infrared_TempVREF[2];
				Infrared_LineVREF[3] = Infrared_TempVREF[3];
				
				EEPROM_WriteOneHalfword( 8,Infrared_TempVREF[0]);//写入eeprom
				EEPROM_WriteOneHalfword(10,Infrared_TempVREF[1]);
				EEPROM_WriteOneHalfword(12,Infrared_TempVREF[2]);
				EEPROM_WriteOneHalfword(14,Infrared_TempVREF[3]);
				Calibration_Flag++;
				LED_Flag = 1;
			}
			else if(Calibration_Flag == 2)//阈值-------------------------------------------------
			{
				Infrare_adc_temp1[0] = 0;
				Infrare_adc_temp2[0] = 0;
				Infrare_adc_temp1[1] = 0;
				Infrare_adc_temp2[1] = 0;
				Infrare_adc_temp1[2] = 0;
				Infrare_adc_temp2[2] = 0;
				Infrare_adc_temp1[3] = 0;
				Infrare_adc_temp2[3] = 0;
				
				for(i=0;i<50;i++)//50组数据求和求平均
				{
					Infrare_A1->GPIO_LOW(Infrare_A1);//关闭探头
					Infrare_A2->GPIO_LOW(Infrare_A2);
					Infrare_A3->GPIO_LOW(Infrare_A3);
					Infrare_A4->GPIO_LOW(Infrare_A4);
					
					delay_ms(1);
					Infrare_adc_temp1[0]+=Infrared_ADC[0];
					Infrare_adc_temp1[1]+=Infrared_ADC[1];
					Infrare_adc_temp1[2]+=Infrared_ADC[2];
					Infrare_adc_temp1[3]+=Infrared_ADC[3];
					
					Infrare_A1->GPIO_HIGH(Infrare_A1);//打开探头
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
				
				//求阈值参考差分电压
				Infrared_TempVREF[0] = (uint16_t)(Infrare_adc_temp2[0] - Infrare_adc_temp1[0]);
				Infrared_TempVREF[1] = (uint16_t)(Infrare_adc_temp2[1] - Infrare_adc_temp1[1]);
				Infrared_TempVREF[2] = (uint16_t)(Infrare_adc_temp2[2] - Infrare_adc_temp1[2]);
				Infrared_TempVREF[3] = (uint16_t)(Infrare_adc_temp2[3] - Infrare_adc_temp1[3]);
				
				Infrared_Threshold_Value[0] = Infrared_TempVREF[0];
				Infrared_Threshold_Value[1] = Infrared_TempVREF[1];
				Infrared_Threshold_Value[2] = Infrared_TempVREF[2];
				Infrared_Threshold_Value[3] = Infrared_TempVREF[3];
				
				EEPROM_WriteOneHalfword(16,Infrared_TempVREF[0]);//写入eeprom
				EEPROM_WriteOneHalfword(18,Infrared_TempVREF[1]);
				EEPROM_WriteOneHalfword(20,Infrared_TempVREF[2]);
				EEPROM_WriteOneHalfword(22,Infrared_TempVREF[3]);
				Calibration_Flag++;
				LED_Flag = 1;
			}else if(Calibration_Flag == 3)//线中点
			{
				Infrare_adc_temp1[0] = 0;
				Infrare_adc_temp2[0] = 0;
				Infrare_adc_temp1[1] = 0;
				Infrare_adc_temp2[1] = 0;
				Infrare_adc_temp1[2] = 0;
				Infrare_adc_temp2[2] = 0;
				Infrare_adc_temp1[3] = 0;
				Infrare_adc_temp2[3] = 0;
				
				for(i=0;i<50;i++)//50组数据求和求平均
				{
					Infrare_A1->GPIO_LOW(Infrare_A1);//关闭探头
					Infrare_A2->GPIO_LOW(Infrare_A2);
					Infrare_A3->GPIO_LOW(Infrare_A3);
					Infrare_A4->GPIO_LOW(Infrare_A4);
					
					delay_ms(1);
					Infrare_adc_temp1[0]+=Infrared_ADC[0];
					Infrare_adc_temp1[1]+=Infrared_ADC[1];
					Infrare_adc_temp1[2]+=Infrared_ADC[2];
					Infrare_adc_temp1[3]+=Infrared_ADC[3];
					
					Infrare_A1->GPIO_HIGH(Infrare_A1);//打开探头
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
				//求线中点偏移
				Infrared_Correction_Value = ((Infrare_adc_temp2[1] - Infrare_adc_temp1[1]) - (Infrare_adc_temp2[2] - Infrare_adc_temp1[2]));
				EEPROM_WriteOneHalfword(24,(uint16_t)Infrared_Correction_Value);
				EEPROM_Data_Save();//保存数据
				Calibration_Flag++;
				Set_LED_State(State_Control);
				break;
			}
		}
	}
}

/**
*@Description :红外探头扫描检测
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
			*(uint64_t*)Infrared_High_Value = *(uint64_t*)Infrared_ADC;//获取无红外发射下电压值
			Infrare_A1->GPIO_LOW(Infrare_A1);
			Infrare_A2->GPIO_LOW(Infrare_A2);
			Infrare_A3->GPIO_LOW(Infrare_A3);
			Infrare_A4->GPIO_LOW(Infrare_A4);
		}else
		{
			Infrared_Start = true;
			*(uint64_t*)Infrared_Low_Value = *(uint64_t*)Infrared_ADC;//获取有红外发射下电压值
			Infrare_A1->GPIO_HIGH(Infrare_A1);
			Infrare_A2->GPIO_HIGH(Infrare_A2);
			Infrare_A3->GPIO_HIGH(Infrare_A3);
			Infrare_A4->GPIO_HIGH(Infrare_A4);
		}
		{
			temp1 = Infrared_High_Value[0]-Infrared_Low_Value[0];
			if(temp1>(Infrared_LineVREF[0]) && temp1<Infrared_BackVREF[0])//探头1在正常范围
			{
				temp1>Infrared_Threshold_Value[0]?(Infrared_Flag|=0x08,Set_LED_Bit_State(0,SET)):(Infrared_Flag&=0xF7,Set_LED_Bit_State(0,RESET));//1
			}
			temp1 = Infrared_High_Value[3]-Infrared_Low_Value[3];
			if(temp1>(Infrared_LineVREF[3]) && temp1<Infrared_BackVREF[3])//探头4在正常范围
			{
				temp1>Infrared_Threshold_Value[3]?(Infrared_Flag|=0x01,Set_LED_Bit_State(3,SET)):(Infrared_Flag&=0xFE,Set_LED_Bit_State(3,RESET));//4
			}
			temp1 = Infrared_High_Value[1]-Infrared_Low_Value[1];
			if(temp1>(Infrared_LineVREF[1]) && temp1<Infrared_BackVREF[1])//探头2在正常范围
			{
				temp1>Infrared_Threshold_Value[1]?(Infrared_Flag|=0x04,Set_LED_Bit_State(1,SET)):(Infrared_Flag&=0xFB,Set_LED_Bit_State(1,RESET));//2
			}
			temp2 = Infrared_High_Value[2]-Infrared_Low_Value[2];
			if(temp2>(Infrared_LineVREF[2]) && temp2<Infrared_BackVREF[2])//探头3在正常范围
			{
				temp2>Infrared_Threshold_Value[2]?(Infrared_Flag|=0x02,Set_LED_Bit_State(2,SET)):(Infrared_Flag&=0xFD,Set_LED_Bit_State(2,RESET));//3
			}
			Line_Pos = (uint8_t)(127+0.15f*(temp1-temp2-Infrared_Correction_Value));//差分数据求偏移比例
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
	Infrare_A1.GPIO_LOW();//关闭探头
	Infrare_A2.GPIO_LOW();
	Infrare_A3.GPIO_LOW();
	Infrare_A4.GPIO_LOW();
	delay_ms(1);
	Value11[0] = Infrared_ADC[0];
	Value11[1] = Infrared_ADC[1];
	Value11[2] = Infrared_ADC[2];
	Value11[3] = Infrared_ADC[3];
	
	Infrare_A1.GPIO_HIGH();//关闭探头
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

