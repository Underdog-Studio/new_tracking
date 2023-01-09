/*****************************************************************
File name:EEPROM驱动

Author:赵华磊

Version:V0.2

Description:提供EEPROM数据写入读取等操作

Others:

Log:2021/6/29
*****************************************************************/
#include "EEPROM_drv.h"

static uint8_t EEPROM_Data[EEPROM_SIZE];


union
{float  a;  
	uint32_t b;
}Temp ;//浮点 uint32 转换共同体

/**
*@Description :EEPROM初始化
**/
void EEPROM_drv_Init(void)
{
	EEPROM_Updata();
}

/**
*@Description :从EEPROM的指定地址写一个字节数据 
*@param       :读取地址
*@param       :待写入数据
**/
void EEPROM_WriteOneByte(uint8_t WriteAddr,uint8_t data)
{
	*(uint8_t*)(&EEPROM_Data[WriteAddr]) = data;
}
/**
*@Description :从EEPROM的指定地址读一个字节数据 
*@param       :读取地址
*@return      :读取到的数据
**/
uint8_t EEPROM_ReadOneByte(uint16_t ReadAddr)
{
	uint8_t temp;
	Flash_Read(EEPROM_ADDR+ReadAddr,(uint8_t*)(&temp),1);
	return temp;
}
/**
*@Description :从EEPROM的指定地址写一个半字数据 
*@param       :读取地址
*@param       :待写入数据
**/
void EEPROM_WriteOneHalfword(uint16_t WriteAddr,uint16_t data)
{
	*(uint16_t*)(&EEPROM_Data[WriteAddr]) = data;
}
/**
*@Description :从EEPROM的指定地址读一个半字数据 
*@param       :读取地址
*@return      :读取到的数据
**/
uint16_t EEPROM_ReadOneHalfword(uint16_t ReadAddr)
{
	uint16_t temp;
	Flash_Read(EEPROM_ADDR+ReadAddr,(uint8_t*)(&temp),2);
	return temp;
}

/**
*@Description :从EEPROM的指定地址写一个字数据 
*@param       :读取地址 高6位页地址(0-63) 低4位页地址偏移(0-15)
*@param       :待写入数据
**/
void EEPROM_WriteOneWord(uint16_t WriteAddr,uint32_t data)
{
	*(uint32_t*)(&EEPROM_Data[WriteAddr]) = data;
}

/**
*@Description :从EEPROM的指定地址读一个字数据 
*@param       :读取地址 高6位页地址(0-63) 低4位页地址偏移(0-15)
*@return      :读取到的数据
**/
uint32_t EEPROM_ReadOneWord(uint16_t ReadAddr)
{
	uint32_t temp;
	Flash_Read(EEPROM_ADDR+ReadAddr,(uint8_t*)(&temp),4);
	return temp;
}

/**
*@Description :从EEPROM的指定地址读一个浮点数据 
*@param       :读取地址
*@return      :读取到的浮点数据
**/
float EEPROM_Read_float(uint16_t ReadAddr)
{
	Flash_Read(EEPROM_ADDR+ReadAddr,(uint8_t*)(&Temp.b),4);
	return Temp.a;
}

/**
*@Description :往EEPROM的指定地址写一个浮点数据 
*@param       :写入地址
*@param       :待写入浮点数据
**/
void EEPROM_Write_float(uint16_t WriteAddr,float data)
{
	Temp.a=data;
	*(uint32_t*)(&EEPROM_Data[WriteAddr]) = Temp.b;
}
/**
*@Description :EEPROM数据保存
**/


void EEPROM_Data_Save(void)
{
	Flash_Erase(EEPROM_ADDR,EEPROM_SIZE);//Flash末尾处保存数据
	Flash_Write(EEPROM_ADDR,EEPROM_Data,EEPROM_SIZE);
}
/**
*@Description :EEPROM数据更新
**/
void EEPROM_Updata(void)
{
	Flash_Read(EEPROM_ADDR,EEPROM_Data,EEPROM_SIZE);
}




