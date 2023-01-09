#include "spi_hal.h"

void spi0_init(void)
{
	spi_parameter_struct spi_init_struct;
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_SPI0);
	
	gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	
	spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;//全双工
	spi_init_struct.device_mode = SPI_MASTER;//主机模式
	spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;//8bit数据
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;//数据传输发生在第二个下降沿
	spi_init_struct.nss = SPI_NSS_SOFT;//软件片选
	spi_init_struct.prescale = SPI_PSC_8;//32分频
	spi_init_struct.endian = SPI_ENDIAN_MSB;//高位在前
	spi_init(SPI0,&spi_init_struct);
	
	spi_fifo_access_size_config(SPI0,SPI_BYTE_ACCESS);//使能数据位
	spi_enable(SPI0);
}

/*
*@ Description :SPI1读写1byte
*@ param       :待发送数据
*@ return      :接收数据
*/

uint8_t spi0_readwrite_byte(uint8_t data)
{
	while(spi_i2s_flag_get(SPI0,SPI_FLAG_TBE)==RESET);
	spi_i2s_data_transmit(SPI0,data);
	while(spi_i2s_flag_get(SPI0,SPI_FLAG_RBNE)==RESET);
	return spi_i2s_data_receive(SPI0);
}

