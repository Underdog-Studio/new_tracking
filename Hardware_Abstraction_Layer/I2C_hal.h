#ifndef __I2C_hal_H
#define __I2C_hal_H

#include "gd32e23x.h"
#include "Software_Register.h"
//����ģʽ:1 �ӻ�ģʽ:0
#define I2C_Master 1
#define I2C_Slaves 0

//�޸Ĵ˺�ѡ��ʹ��I2C0
#define I2C0_hal 1
//�޸Ĵ˺�ѡ��I2C0����ģʽ
#define I2C0_Mode I2C_Slaves

#define I2C0_Slave_Addres 0x76 //�ӻ���ַ

#define I2C0_Speed 350000 //���������ٶ�

//�޸Ĵ˺�ѡ��ʹ��I2C1
#define I2C1_hal 0
//�޸Ĵ˺�ѡ��I2C1����ģʽ
#define I2C1_Mode I2C_Slaves

#define I2C1_Slave_Addres 0x0f //�ӻ���ַ

#define I2C1_Speed 350000 //���������ٶ�


void I2C_hal_Init(void);

uint8_t I2C_Register_Write_1Byte(uint8_t Device_address,uint8_t Register_address,uint8_t data);
uint8_t I2C_Register_Read_1Byte(uint8_t Device_address,uint8_t Register_address,uint8_t *data);
uint8_t I2C_Register_Read_nByte(uint8_t Device_address,uint8_t Register_address,uint8_t *data,uint8_t num);

void I2C0_Rsgister_Register(Software_Register *Regisetr);

void I2C0_Scan(void);
#endif




