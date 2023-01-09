/***************************
File name:ADC�ɼ�����

Author:�Ի���

Version:V0.1

Description:�ṩADC�ɼ�����

Others:

Log:2022/2/24
****************************/
#include "adc_hal.h"
static uint16_t ADC_value[8]={0,0,0,0,0,0,0,0};

void adc_init(void)
{
	//��ʼ��GPIO
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);//adc
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);//adc

	//��ʼ��dma
	dma_parameter_struct dma_data_parameter;

	rcu_periph_clock_enable(RCU_DMA);
	
	dma_deinit(DMA_CH0);
	
	dma_data_parameter.periph_addr=(uint32_t)(&ADC_RDATA);//dma�����ַ(����Դ��ַ)
	dma_data_parameter.periph_inc=DMA_PERIPH_INCREASE_DISABLE;//����Դ��ַ����
	dma_data_parameter.memory_addr=(uint32_t)(ADC_value);//dma�洢��ַ(����Ŀ���ַ)
	dma_data_parameter.memory_inc=DMA_MEMORY_INCREASE_ENABLE;//����Ŀ���ַ����

	dma_data_parameter.periph_width=DMA_PERIPHERAL_WIDTH_16BIT;//����Դλ��
	dma_data_parameter.memory_width=DMA_MEMORY_WIDTH_16BIT;//����Ŀ��λ��λ��

	dma_data_parameter.direction=DMA_PERIPHERAL_TO_MEMORY;//���ݴ��䷽�� ����-->�ڴ�
	dma_data_parameter.number=5;//dmaһ������Ҫ��������ݸ���
	dma_data_parameter.priority=DMA_PRIORITY_HIGH;//����dma���ݴ�������ȼ�
	
	dma_init(DMA_CH0,&dma_data_parameter);
	
	dma_circulation_enable(DMA_CH0);
	dma_memory_to_memory_disable(DMA_CH0);
  dma_channel_enable(DMA_CH0);
	
	
	//��ʼ��adc
	rcu_periph_clock_enable(RCU_ADC);
	rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6); //9M

	adc_deinit();//��λADC
		
	adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE); //����ת��
	adc_special_function_config(ADC_SCAN_MODE, ENABLE);	//ɨ��ģʽ
	
	adc_data_alignment_config(ADC_DATAALIGN_RIGHT);  //�Ҷ���
	adc_channel_length_config(ADC_REGULAR_CHANNEL,5); //����ת����ʽ������ͨ�����г���
	adc_regular_channel_config(0, ADC_CHANNEL_0 , ADC_SAMPLETIME_55POINT5);    //��ص�ѹ
	adc_regular_channel_config(1, ADC_CHANNEL_1 , ADC_SAMPLETIME_55POINT5);    //���1
	adc_regular_channel_config(2, ADC_CHANNEL_2 , ADC_SAMPLETIME_55POINT5);    //���2
	adc_regular_channel_config(3, ADC_CHANNEL_3 , ADC_SAMPLETIME_55POINT5);    //���3
  adc_regular_channel_config(4, ADC_CHANNEL_4 , ADC_SAMPLETIME_55POINT5);    //���4

	
	adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE); //�ⲿ����ʹ��
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC_EXTTRIG_REGULAR_NONE);  //�������
	
	
	adc_enable();//����ADC
	
	adc_calibration_enable();//ADCУ��
	adc_dma_mode_enable(); //ADC����DMAģʽ
	
	adc_software_trigger_enable(ADC_REGULAR_CHANNEL);//�������ʹ��
}

uint16_t* get_value(uint8_t num)
{
	return ADC_value+num;
}



