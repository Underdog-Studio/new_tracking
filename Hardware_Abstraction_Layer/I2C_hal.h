#ifndef __I2C_hal_H
#define __I2C_hal_H

#include "gd32e23x.h"
#include "Software_Register.h"
//主机模式:1 从机模式:0
#define I2C_Master 1
#define I2C_Slaves 0

//修改此宏选择使用I2C0
#define I2C0_hal 1
//修改此宏选择I2C0工作模式
#define I2C0_Mode I2C_Slaves

#define I2C0_Slave_Addres 0x76 //从机地址

#define I2C0_Speed 350000 //主机工作速度

//修改此宏选择使用I2C1
#define I2C1_hal 0
//修改此宏选择I2C1工作模式
#define I2C1_Mode I2C_Slaves

#define I2C1_Slave_Addres 0x0f //从机地址

#define I2C1_Speed 350000 //主机工作速度


void I2C_hal_Init(void);

uint8_t I2C_Register_Write_1Byte(uint8_t Device_address,uint8_t Register_address,uint8_t data);
uint8_t I2C_Register_Read_1Byte(uint8_t Device_address,uint8_t Register_address,uint8_t *data);
uint8_t I2C_Register_Read_nByte(uint8_t Device_address,uint8_t Register_address,uint8_t *data,uint8_t num);

void I2C0_Rsgister_Register(Software_Register *Regisetr);

void I2C0_Scan(void);
#endif




