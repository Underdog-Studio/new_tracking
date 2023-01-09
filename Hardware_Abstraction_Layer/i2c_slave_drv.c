/***************************
File name:I2C从机驱动

Author:赵华磊

Version:V0.1

Description:提供I2C从机驱动

Others:

Log:2022/1/13
****************************/
#include "i2c_slave_drv.h"
#include "Software_Register.h"


static void I2C_GPIO_Config(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);

	gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_6);//SCL
	gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_7);//SDA
	
	gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	
	gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
}

static void I2C_Config(void)
{
	rcu_periph_clock_enable(RCU_I2C0);
	
	i2c_software_reset_config(I2C0, I2C_SRESET_SET);
	i2c_software_reset_config(I2C0, I2C_SRESET_RESET);
	
	i2c_clock_config(I2C0, I2C_Speed, I2C_DTCY_2);//设置速度
	
	i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C_Slave_Addres);//设置地址
	
	i2c_enable(I2C0);
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);
	
	
	i2c_interrupt_enable(I2C0, I2C_INT_ERR);
	i2c_interrupt_enable(I2C0, I2C_INT_EV);
	i2c_interrupt_enable(I2C0, I2C_INT_BUF);
	nvic_irq_enable(I2C0_EV_IRQn,1);	
	nvic_irq_enable(I2C0_ER_IRQn,1);	
}

void i2c_slave_init(void)
{
	I2C_GPIO_Config();
	I2C_Config();
}

uint8_t Register_Address=0;//待操作寄存器地址
uint8_t Addsend_Flag=0,Register_Address_Flag=0;//收到地址标志


void I2C0_EV_IRQHandler(void)
{
	if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_ADDSEND))//收到地址请求
	{
		Addsend_Flag=1;//记录收到地址
		i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_ADDSEND);
	}else if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_RBNE))//收到主机写入
	{
		if(Addsend_Flag&&(Register_Address_Flag==0))//收到地址后写入
		{
			Register_Address=i2c_data_receive(I2C0);//记录寄存器地址
			Register_Address_Flag=1;//接收到寄存器地址标志
			Addsend_Flag=0;//清除收到地址标志
		}else//主机写入寄存器
		{
			Write_Software_Register(Register_Address,i2c_data_receive(I2C0));//软件寄存器写入
			Register_Address++;
		}
		i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_RBNE);
	}else if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE))//收到主机读取
	{
		if(Addsend_Flag)//收到地址后读取
		{
			i2c_data_transmit(I2C0, Read_Software_Register(Register_Address++));//软件寄存器读取
			Addsend_Flag=0;//清除收到地址标志
		}else
		{
			i2c_data_transmit(I2C0, Read_Software_Register(Register_Address++));//软件寄存器读取
		}
		i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_TBE);
	}else if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_STPDET))//收到主机停止
	{
		Register_Address=0;
		Addsend_Flag=0;
		Register_Address_Flag=0;
		i2c_enable(I2C0);
		i2c_interrupt_disable(I2C0, I2C_INT_ERR|I2C_INT_BUF|I2C_INT_EV);
		i2c_interrupt_enable(I2C0, I2C_INT_ERR|I2C_INT_BUF|I2C_INT_EV);
	}
	
}

void I2C0_ER_IRQHandler(void)
{
	if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_AERR))//未收到ACK
	{
		i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_AERR);
		
		Register_Address=0;
		Addsend_Flag=0;
		Register_Address_Flag=0;
		
	}
	
}
















