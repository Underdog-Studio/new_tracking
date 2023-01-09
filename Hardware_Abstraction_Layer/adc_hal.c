/***************************
File name:ADC采集驱动

Author:赵华磊

Version:V0.1

Description:提供ADC采集驱动

Others:

Log:2022/2/24
****************************/
#include "adc_hal.h"
static uint16_t ADC_value[8]={0,0,0,0,0,0,0,0};

void adc_init(void)
{
	//初始化GPIO
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);//adc
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);//adc

	//初始化dma
	dma_parameter_struct dma_data_parameter;

	rcu_periph_clock_enable(RCU_DMA);
	
	dma_deinit(DMA_CH0);
	
	dma_data_parameter.periph_addr=(uint32_t)(&ADC_RDATA);//dma外设地址(数据源地址)
	dma_data_parameter.periph_inc=DMA_PERIPH_INCREASE_DISABLE;//数据源地址自增
	dma_data_parameter.memory_addr=(uint32_t)(ADC_value);//dma存储地址(数据目标地址)
	dma_data_parameter.memory_inc=DMA_MEMORY_INCREASE_ENABLE;//数据目标地址自增

	dma_data_parameter.periph_width=DMA_PERIPHERAL_WIDTH_16BIT;//数据源位宽
	dma_data_parameter.memory_width=DMA_MEMORY_WIDTH_16BIT;//数据目标位置位宽

	dma_data_parameter.direction=DMA_PERIPHERAL_TO_MEMORY;//数据传输方向 外设-->内存
	dma_data_parameter.number=5;//dma一个周期要传输的数据个数
	dma_data_parameter.priority=DMA_PRIORITY_HIGH;//设置dma数据传输高优先级
	
	dma_init(DMA_CH0,&dma_data_parameter);
	
	dma_circulation_enable(DMA_CH0);
	dma_memory_to_memory_disable(DMA_CH0);
  dma_channel_enable(DMA_CH0);
	
	
	//初始化adc
	rcu_periph_clock_enable(RCU_ADC);
	rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6); //9M

	adc_deinit();//复位ADC
		
	adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE); //连续转换
	adc_special_function_config(ADC_SCAN_MODE, ENABLE);	//扫描模式
	
	adc_data_alignment_config(ADC_DATAALIGN_RIGHT);  //右对齐
	adc_channel_length_config(ADC_REGULAR_CHANNEL,5); //设置转换方式，规则通道序列长度
	adc_regular_channel_config(0, ADC_CHANNEL_0 , ADC_SAMPLETIME_55POINT5);    //电池电压
	adc_regular_channel_config(1, ADC_CHANNEL_1 , ADC_SAMPLETIME_55POINT5);    //舵机1
	adc_regular_channel_config(2, ADC_CHANNEL_2 , ADC_SAMPLETIME_55POINT5);    //舵机2
	adc_regular_channel_config(3, ADC_CHANNEL_3 , ADC_SAMPLETIME_55POINT5);    //舵机3
  adc_regular_channel_config(4, ADC_CHANNEL_4 , ADC_SAMPLETIME_55POINT5);    //舵机4

	
	adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE); //外部触发使能
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC_EXTTRIG_REGULAR_NONE);  //软件触发
	
	
	adc_enable();//开启ADC
	
	adc_calibration_enable();//ADC校验
	adc_dma_mode_enable(); //ADC开启DMA模式
	
	adc_software_trigger_enable(ADC_REGULAR_CHANNEL);//软件触发使能
}

uint16_t* get_value(uint8_t num)
{
	return ADC_value+num;
}



