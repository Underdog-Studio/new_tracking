/*****************************************************************
File name:EEPROM����

Author:�Ի���

Version:V0.2

Description:�ṩEEPROM����д���ȡ�Ȳ���

Others:

Log:2021/6/29
*****************************************************************/
#include "EEPROM_drv.h"

static uint8_t EEPROM_Data[EEPROM_SIZE];


union
{float  a;  
	uint32_t b;
}Temp ;//���� uint32 ת����ͬ��

/**
*@Description :EEPROM��ʼ��
**/
void EEPROM_drv_Init(void)
{
	EEPROM_Updata();
}

/**
*@Description :��EEPROM��ָ����ַдһ���ֽ����� 
*@param       :��ȡ��ַ
*@param       :��д������
**/
void EEPROM_WriteOneByte(uint8_t WriteAddr,uint8_t data)
{
	*(uint8_t*)(&EEPROM_Data[WriteAddr]) = data;
}
/**
*@Description :��EEPROM��ָ����ַ��һ���ֽ����� 
*@param       :��ȡ��ַ
*@return      :��ȡ��������
**/
uint8_t EEPROM_ReadOneByte(uint16_t ReadAddr)
{
	uint8_t temp;
	Flash_Read(EEPROM_ADDR+ReadAddr,(uint8_t*)(&temp),1);
	return temp;
}
/**
*@Description :��EEPROM��ָ����ַдһ���������� 
*@param       :��ȡ��ַ
*@param       :��д������
**/
void EEPROM_WriteOneHalfword(uint16_t WriteAddr,uint16_t data)
{
	*(uint16_t*)(&EEPROM_Data[WriteAddr]) = data;
}
/**
*@Description :��EEPROM��ָ����ַ��һ���������� 
*@param       :��ȡ��ַ
*@return      :��ȡ��������
**/
uint16_t EEPROM_ReadOneHalfword(uint16_t ReadAddr)
{
	uint16_t temp;
	Flash_Read(EEPROM_ADDR+ReadAddr,(uint8_t*)(&temp),2);
	return temp;
}

/**
*@Description :��EEPROM��ָ����ַдһ�������� 
*@param       :��ȡ��ַ ��6λҳ��ַ(0-63) ��4λҳ��ַƫ��(0-15)
*@param       :��д������
**/
void EEPROM_WriteOneWord(uint16_t WriteAddr,uint32_t data)
{
	*(uint32_t*)(&EEPROM_Data[WriteAddr]) = data;
}

/**
*@Description :��EEPROM��ָ����ַ��һ�������� 
*@param       :��ȡ��ַ ��6λҳ��ַ(0-63) ��4λҳ��ַƫ��(0-15)
*@return      :��ȡ��������
**/
uint32_t EEPROM_ReadOneWord(uint16_t ReadAddr)
{
	uint32_t temp;
	Flash_Read(EEPROM_ADDR+ReadAddr,(uint8_t*)(&temp),4);
	return temp;
}

/**
*@Description :��EEPROM��ָ����ַ��һ���������� 
*@param       :��ȡ��ַ
*@return      :��ȡ���ĸ�������
**/
float EEPROM_Read_float(uint16_t ReadAddr)
{
	Flash_Read(EEPROM_ADDR+ReadAddr,(uint8_t*)(&Temp.b),4);
	return Temp.a;
}

/**
*@Description :��EEPROM��ָ����ַдһ���������� 
*@param       :д���ַ
*@param       :��д�븡������
**/
void EEPROM_Write_float(uint16_t WriteAddr,float data)
{
	Temp.a=data;
	*(uint32_t*)(&EEPROM_Data[WriteAddr]) = Temp.b;
}
/**
*@Description :EEPROM���ݱ���
**/


void EEPROM_Data_Save(void)
{
	Flash_Erase(EEPROM_ADDR,EEPROM_SIZE);//Flashĩβ����������
	Flash_Write(EEPROM_ADDR,EEPROM_Data,EEPROM_SIZE);
}
/**
*@Description :EEPROM���ݸ���
**/
void EEPROM_Updata(void)
{
	Flash_Read(EEPROM_ADDR,EEPROM_Data,EEPROM_SIZE);
}




