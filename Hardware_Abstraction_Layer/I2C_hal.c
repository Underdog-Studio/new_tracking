#include "I2C_hal.h"

Software_Register *I2C0_Register;//软件寄存器
Software_Register *I2C1_Register;//软件寄存器

static void I2C_GPIO_Config(void)
{
	#if I2C0_hal
		rcu_periph_clock_enable(RCU_GPIOB);

		gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_6);//SCL
		gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_7);//SDA
		
		gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
		gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
		
		gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
		gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
	#endif
	#if I2C1_hal
		rcu_periph_clock_enable(RCU_GPIOA);

		gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_0);//SCL
		gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_1);//SDA
		
		gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
		gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
		
		gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1);
		gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
	#endif
}

static void I2C_Config(void)
{
	#if I2C0_hal
		rcu_periph_clock_enable(RCU_I2C0);
		i2c_software_reset_config(I2C0, I2C_SRESET_SET);
		i2c_software_reset_config(I2C0, I2C_SRESET_RESET);
		i2c_clock_config(I2C0, I2C0_Speed, I2C_DTCY_2);//设置速度
		#if I2C0_Mode==I2C_Master
			i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x72);
		#elif I2C0_Mode==I2C_Slaves
			i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_Slave_Addres);//设置从机地址
			I2C0_Register = 0;
			i2c_interrupt_enable(I2C0, I2C_INT_ERR);
			i2c_interrupt_enable(I2C0, I2C_INT_EV);
			i2c_interrupt_enable(I2C0, I2C_INT_BUF);
			nvic_irq_enable(I2C0_EV_IRQn,1);	
			nvic_irq_enable(I2C0_ER_IRQn,1);
		#endif
		i2c_enable(I2C0);
		i2c_ack_config(I2C0, I2C_ACK_ENABLE);
	#endif
	#if I2C1_hal
		rcu_periph_clock_enable(RCU_I2C1);
		i2c_software_reset_config(I2C1, I2C_SRESET_SET);
		i2c_software_reset_config(I2C1, I2C_SRESET_RESET);
		i2c_clock_config(I2C1, I2C1_Speed, I2C_DTCY_2);//设置速度
		#if I2C1_Mode==I2C_Master
			i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x72);
		#elif I2C1_Mode==I2C_Slaves
			i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_Slave_Addres);//设置从机地址
			I2C1_Register = 0;
			i2c_interrupt_enable(I2C1, I2C_INT_ERR);
			i2c_interrupt_enable(I2C1, I2C_INT_EV);
			i2c_interrupt_enable(I2C1, I2C_INT_BUF);
			nvic_irq_enable(I2C0_EV_IRQn,1);	
			nvic_irq_enable(I2C0_ER_IRQn,1);
		#endif
		i2c_enable(I2C0);
		i2c_ack_config(I2C0, I2C_ACK_ENABLE);
	#endif
}

void I2C_hal_Init(void)
{
	I2C_GPIO_Config();
	I2C_Config();
}

#if I2C0_Mode==I2C_Master
uint8_t I2C_Register_Write_1Byte(uint8_t Device_address,uint8_t Register_address,uint8_t data)
{
	while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));//i2c是否空闲
	i2c_start_on_bus(I2C0);//起始信号
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//起始信号成功发送
	
	i2c_master_addressing(I2C0, Device_address, I2C_TRANSMITTER);//设备寻址
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));//寻址成功发送
	i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);//地址发送成功标志位清零
	
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//I2C_data 是否为空
	i2c_data_transmit(I2C0, Register_address);//写寄存器地址
	
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE)){};//I2C_data 是否为空
  i2c_data_transmit(I2C0, data);//写数据
	
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//等待传输完成
	i2c_stop_on_bus(I2C0);//停止信号
	while(I2C_CTL0(I2C0)&0x0200);
		
	return 1;
}

uint8_t I2C_Register_Read_1Byte(uint8_t Device_address,uint8_t Register_address,uint8_t *data)
{
	while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));//i2c是否空闲
	i2c_start_on_bus(I2C0);//起始信号
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//起始信号成功发送
	
	i2c_master_addressing(I2C0, Device_address, I2C_TRANSMITTER);//设备寻址
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));//地址成功发送
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//地址发送成功标志位清零
	
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//I2C_data 是否为空

	i2c_data_transmit(I2C0,Register_address);//写寄存器地址
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//等待传输完成
	
	i2c_start_on_bus(I2C0);//起始信号
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//起始信号成功发送
	
	i2c_master_addressing(I2C0, Device_address, I2C_RECEIVER);//设备寻址
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));//地址成功发送
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//地址发送成功标志位清零
	
	i2c_ack_config(I2C0, I2C_ACK_DISABLE);			/* 设置为非应答 NACK, 要在清除FLAG_ADDSEND前*/

  i2c_stop_on_bus(I2C0);							/* 发送一个停止位到I2C总线 */
  while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));		/* 等待数据寄存器可读 */
  *data  = i2c_data_receive(I2C0);			/* 读出接收到的数据 */
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);			/* 使能应答 ACK, */ 	
	return 1;
}

uint8_t I2C_Register_Read_nByte(uint8_t Device_address,uint8_t Register_address,uint8_t *data,uint8_t num)
{
	uint8_t i=0;
	while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));//i2c是否空闲
	i2c_start_on_bus(I2C0);//起始信号
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//起始信号成功发送
	
	i2c_master_addressing(I2C0, Device_address, I2C_TRANSMITTER);//设备寻址
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));//地址成功发送
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//地址发送成功标志位清零
	
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//I2C_data 是否为空

	i2c_data_transmit(I2C0,Register_address);//写寄存器地址
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//等待传输完成
	
	i2c_start_on_bus(I2C0);//起始信号
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//起始信号成功发送
	
	i2c_master_addressing(I2C0, Device_address, I2C_RECEIVER);//设备寻址
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));//地址成功发送
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//地址发送成功标志位清零

	while(num>1&&i<(num-1))
	{
		while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));		/* 等待数据寄存器可读 */
		data[i++] = i2c_data_receive(I2C0);			/* 读出接收到的数据 */
	}
	
	i2c_ack_config(I2C0, I2C_ACK_DISABLE);			/* 设置为非应答 NACK, 要在清除FLAG_ADDSEND前*/
	
  i2c_stop_on_bus(I2C0);							/* 发送一个停止位到I2C总线 */
  while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));		/* 等待数据寄存器可读 */
  data[i]  = i2c_data_receive(I2C0);			/* 读出接收到的数据 */
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);			/* 使能应答 ACK, */ 
	return 1;
}

uint8_t I2C_ID[127];
void I2C0_Scan(void)
{
	uint8_t Count=0,count1;
	uint16_t TTTT=0;
	//i2c_stop_on_bus(I2C0);
	while((Count++)<255)
	{
		while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));//i2c是否空闲
		i2c_start_on_bus(I2C0);//起始信号
		while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//起始信号成功发送
		
		i2c_master_addressing(I2C0, Count, I2C_RECEIVER);//设备寻址
			
		while(1)
		{
			if(i2c_flag_get(I2C0, I2C_FLAG_ADDSEND))
			{
				I2C_ID[count1++] = Count;
				i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//地址发送成功标志位清零
				i2c_stop_on_bus(I2C0);
				break;
			}
			if(TTTT++>5000)
			{
				i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//地址发送成功标志位清零
				i2c_stop_on_bus(I2C0);
				TTTT=0;
				break;
			}
		}
	}
}

#endif

#if I2C0_Mode == I2C_Slaves
	uint8_t Register_Address=0;//待操作寄存器地址
	uint8_t Addsend_Flag=0,Register_Address_Flag=0;//收到地址标志
	/**
	*@Description :I2C事件中断
	**/
	void I2C0_EV_IRQHandler(void)
	{
		uint8_t Temp;
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
				Temp = i2c_data_receive(I2C0);
				I2C0_Register!=0?Software_Register_Write(I2C0_Register,Register_Address,&Temp):0;//软件寄存器写入
				Register_Address++;
			}
			i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_RBNE);
		}else if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE))//收到主机读取
		{
			if(Addsend_Flag)//收到地址后读取
			{
				I2C0_Register!=0?Software_Register_Read(I2C0_Register,Register_Address++,&Temp):0;//软件寄存器读取
				i2c_data_transmit(I2C0,Temp);
				Addsend_Flag=0;//清除收到地址标志
			}else
			{
				I2C0_Register!=0?Software_Register_Read(I2C0_Register,Register_Address++,&Temp):0;//软件寄存器读取
				i2c_data_transmit(I2C0,Temp);
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

	/**
	*@Description :I2C错误中断
	**/
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
	/**
	*@Description :注册I2C0从机软件寄存器
	**/
	void I2C0_Rsgister_Register(Software_Register *Regisetr)
	{
		I2C0_Register = Regisetr;
	}
#endif

