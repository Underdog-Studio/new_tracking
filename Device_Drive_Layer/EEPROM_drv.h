#ifndef __EEPROM_drv_H
#define __EEPROM_drv_H

#include "gd32e23x.h"
#include "System_time.h"
#include "Flash_hal.h"


#define EEPROM_SIZE PAGE_SIZE*1 //1������
#define EEPROM_ADDR 0x8000000+Flash_SIZE-(EEPROM_SIZE) //β��

void EEPROM_drv_Init(void);

uint8_t Parameters_Loading(void);//��������


//�����ȸ������ݲ���
float EEPROM_Read_float(uint16_t ReadAddr);
void EEPROM_Write_float(uint16_t ReadAddr,float data);

//�����ݲ���
uint32_t EEPROM_ReadOneWord(uint16_t ReadAddr);
void EEPROM_WriteOneWord(uint16_t WriteAddr,uint32_t data);

//�������ݲ���
uint16_t EEPROM_ReadOneHalfword(uint16_t ReadAddr);
void EEPROM_WriteOneHalfword(uint16_t WriteAddr,uint16_t data);

//�ֽ����ݲ���
void EEPROM_WriteOneByte(uint8_t WriteAddr,uint8_t data);
uint8_t EEPROM_ReadOneByte(uint16_t ReadAddr);

void EEPROM_Updata(void);
void EEPROM_Data_Save(void);


#endif

