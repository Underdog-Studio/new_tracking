/***************************
File name:I2C�ӻ�����

Author:�Ի���

Version:V0.1

Description:�ṩI2C�ӻ�����

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
	
	i2c_clock_config(I2C0, I2C_Speed, I2C_DTCY_2);//�����ٶ�
	
	i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C_Slave_Addres);//���õ�ַ
	
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

uint8_t Register_Address=0;//�������Ĵ�����ַ
uint8_t Addsend_Flag=0,Register_Address_Flag=0;//�յ���ַ��־


void I2C0_EV_IRQHandler(void)
{
	if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_ADDSEND))//�յ���ַ����
	{
		Addsend_Flag=1;//��¼�յ���ַ
		i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_ADDSEND);
	}else if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_RBNE))//�յ�����д��
	{
		if(Addsend_Flag&&(Register_Address_Flag==0))//�յ���ַ��д��
		{
			Register_Address=i2c_data_receive(I2C0);//��¼�Ĵ�����ַ
			Register_Address_Flag=1;//���յ��Ĵ�����ַ��־
			Addsend_Flag=0;//����յ���ַ��־
		}else//����д��Ĵ���
		{
			Write_Software_Register(Register_Address,i2c_data_receive(I2C0));//����Ĵ���д��
			Register_Address++;
		}
		i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_RBNE);
	}else if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE))//�յ�������ȡ
	{
		if(Addsend_Flag)//�յ���ַ���ȡ
		{
			i2c_data_transmit(I2C0, Read_Software_Register(Register_Address++));//����Ĵ�����ȡ
			Addsend_Flag=0;//����յ���ַ��־
		}else
		{
			i2c_data_transmit(I2C0, Read_Software_Register(Register_Address++));//����Ĵ�����ȡ
		}
		i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_TBE);
	}else if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_STPDET))//�յ�����ֹͣ
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
	if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_AERR))//δ�յ�ACK
	{
		i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_AERR);
		
		Register_Address=0;
		Addsend_Flag=0;
		Register_Address_Flag=0;
		
	}
	
}
















