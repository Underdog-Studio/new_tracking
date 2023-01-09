#include "I2C_hal.h"

Software_Register *I2C0_Register;//����Ĵ���
Software_Register *I2C1_Register;//����Ĵ���

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
		i2c_clock_config(I2C0, I2C0_Speed, I2C_DTCY_2);//�����ٶ�
		#if I2C0_Mode==I2C_Master
			i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x72);
		#elif I2C0_Mode==I2C_Slaves
			i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_Slave_Addres);//���ôӻ���ַ
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
		i2c_clock_config(I2C1, I2C1_Speed, I2C_DTCY_2);//�����ٶ�
		#if I2C1_Mode==I2C_Master
			i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x72);
		#elif I2C1_Mode==I2C_Slaves
			i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_Slave_Addres);//���ôӻ���ַ
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
	while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));//i2c�Ƿ����
	i2c_start_on_bus(I2C0);//��ʼ�ź�
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//��ʼ�źųɹ�����
	
	i2c_master_addressing(I2C0, Device_address, I2C_TRANSMITTER);//�豸Ѱַ
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));//Ѱַ�ɹ�����
	i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);//��ַ���ͳɹ���־λ����
	
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//I2C_data �Ƿ�Ϊ��
	i2c_data_transmit(I2C0, Register_address);//д�Ĵ�����ַ
	
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE)){};//I2C_data �Ƿ�Ϊ��
  i2c_data_transmit(I2C0, data);//д����
	
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//�ȴ��������
	i2c_stop_on_bus(I2C0);//ֹͣ�ź�
	while(I2C_CTL0(I2C0)&0x0200);
		
	return 1;
}

uint8_t I2C_Register_Read_1Byte(uint8_t Device_address,uint8_t Register_address,uint8_t *data)
{
	while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));//i2c�Ƿ����
	i2c_start_on_bus(I2C0);//��ʼ�ź�
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//��ʼ�źųɹ�����
	
	i2c_master_addressing(I2C0, Device_address, I2C_TRANSMITTER);//�豸Ѱַ
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));//��ַ�ɹ�����
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//��ַ���ͳɹ���־λ����
	
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//I2C_data �Ƿ�Ϊ��

	i2c_data_transmit(I2C0,Register_address);//д�Ĵ�����ַ
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//�ȴ��������
	
	i2c_start_on_bus(I2C0);//��ʼ�ź�
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//��ʼ�źųɹ�����
	
	i2c_master_addressing(I2C0, Device_address, I2C_RECEIVER);//�豸Ѱַ
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));//��ַ�ɹ�����
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//��ַ���ͳɹ���־λ����
	
	i2c_ack_config(I2C0, I2C_ACK_DISABLE);			/* ����Ϊ��Ӧ�� NACK, Ҫ�����FLAG_ADDSENDǰ*/

  i2c_stop_on_bus(I2C0);							/* ����һ��ֹͣλ��I2C���� */
  while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));		/* �ȴ����ݼĴ����ɶ� */
  *data  = i2c_data_receive(I2C0);			/* �������յ������� */
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);			/* ʹ��Ӧ�� ACK, */ 	
	return 1;
}

uint8_t I2C_Register_Read_nByte(uint8_t Device_address,uint8_t Register_address,uint8_t *data,uint8_t num)
{
	uint8_t i=0;
	while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));//i2c�Ƿ����
	i2c_start_on_bus(I2C0);//��ʼ�ź�
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//��ʼ�źųɹ�����
	
	i2c_master_addressing(I2C0, Device_address, I2C_TRANSMITTER);//�豸Ѱַ
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));//��ַ�ɹ�����
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//��ַ���ͳɹ���־λ����
	
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//I2C_data �Ƿ�Ϊ��

	i2c_data_transmit(I2C0,Register_address);//д�Ĵ�����ַ
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));//�ȴ��������
	
	i2c_start_on_bus(I2C0);//��ʼ�ź�
	while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//��ʼ�źųɹ�����
	
	i2c_master_addressing(I2C0, Device_address, I2C_RECEIVER);//�豸Ѱַ
	while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));//��ַ�ɹ�����
	i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//��ַ���ͳɹ���־λ����

	while(num>1&&i<(num-1))
	{
		while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));		/* �ȴ����ݼĴ����ɶ� */
		data[i++] = i2c_data_receive(I2C0);			/* �������յ������� */
	}
	
	i2c_ack_config(I2C0, I2C_ACK_DISABLE);			/* ����Ϊ��Ӧ�� NACK, Ҫ�����FLAG_ADDSENDǰ*/
	
  i2c_stop_on_bus(I2C0);							/* ����һ��ֹͣλ��I2C���� */
  while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));		/* �ȴ����ݼĴ����ɶ� */
  data[i]  = i2c_data_receive(I2C0);			/* �������յ������� */
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);			/* ʹ��Ӧ�� ACK, */ 
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
		while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));//i2c�Ƿ����
		i2c_start_on_bus(I2C0);//��ʼ�ź�
		while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));//��ʼ�źųɹ�����
		
		i2c_master_addressing(I2C0, Count, I2C_RECEIVER);//�豸Ѱַ
			
		while(1)
		{
			if(i2c_flag_get(I2C0, I2C_FLAG_ADDSEND))
			{
				I2C_ID[count1++] = Count;
				i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//��ַ���ͳɹ���־λ����
				i2c_stop_on_bus(I2C0);
				break;
			}
			if(TTTT++>5000)
			{
				i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);//��ַ���ͳɹ���־λ����
				i2c_stop_on_bus(I2C0);
				TTTT=0;
				break;
			}
		}
	}
}

#endif

#if I2C0_Mode == I2C_Slaves
	uint8_t Register_Address=0;//�������Ĵ�����ַ
	uint8_t Addsend_Flag=0,Register_Address_Flag=0;//�յ���ַ��־
	/**
	*@Description :I2C�¼��ж�
	**/
	void I2C0_EV_IRQHandler(void)
	{
		uint8_t Temp;
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
				Temp = i2c_data_receive(I2C0);
				I2C0_Register!=0?Software_Register_Write(I2C0_Register,Register_Address,&Temp):0;//����Ĵ���д��
				Register_Address++;
			}
			i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_RBNE);
		}else if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE))//�յ�������ȡ
		{
			if(Addsend_Flag)//�յ���ַ���ȡ
			{
				I2C0_Register!=0?Software_Register_Read(I2C0_Register,Register_Address++,&Temp):0;//����Ĵ�����ȡ
				i2c_data_transmit(I2C0,Temp);
				Addsend_Flag=0;//����յ���ַ��־
			}else
			{
				I2C0_Register!=0?Software_Register_Read(I2C0_Register,Register_Address++,&Temp):0;//����Ĵ�����ȡ
				i2c_data_transmit(I2C0,Temp);
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

	/**
	*@Description :I2C�����ж�
	**/
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
	/**
	*@Description :ע��I2C0�ӻ�����Ĵ���
	**/
	void I2C0_Rsgister_Register(Software_Register *Regisetr)
	{
		I2C0_Register = Regisetr;
	}
#endif

