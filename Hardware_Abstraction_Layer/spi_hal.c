#include "spi_hal.h"

void spi0_init(void)
{
	spi_parameter_struct spi_init_struct;
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_SPI0);
	
	gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	
	spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;//ȫ˫��
	spi_init_struct.device_mode = SPI_MASTER;//����ģʽ
	spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;//8bit����
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;//���ݴ��䷢���ڵڶ����½���
	spi_init_struct.nss = SPI_NSS_SOFT;//���Ƭѡ
	spi_init_struct.prescale = SPI_PSC_8;//32��Ƶ
	spi_init_struct.endian = SPI_ENDIAN_MSB;//��λ��ǰ
	spi_init(SPI0,&spi_init_struct);
	
	spi_fifo_access_size_config(SPI0,SPI_BYTE_ACCESS);//ʹ������λ
	spi_enable(SPI0);
}

/*
*@ Description :SPI1��д1byte
*@ param       :����������
*@ return      :��������
*/

uint8_t spi0_readwrite_byte(uint8_t data)
{
	while(spi_i2s_flag_get(SPI0,SPI_FLAG_TBE)==RESET);
	spi_i2s_data_transmit(SPI0,data);
	while(spi_i2s_flag_get(SPI0,SPI_FLAG_RBNE)==RESET);
	return spi_i2s_data_receive(SPI0);
}

