#include "flash_hal.h"

/**
*@Description :����ָ���ռ�flash
*@param       :������ռ��׵�ַ
*@param       :������Ĵ�С
*@return      :�����Ƿ�ɹ�
**/
uint8_t Flash_Erase(uint32_t First_address,uint32_t size)
{
	fmc_unlock();     // ����flash
	fmc_state_enum FLASHStatus = FMC_READY;
	if(size > Flash_SIZE-(First_address-0x8000000))
	{
		fmc_lock(); //����flash
		return 0;//������Ĵ�С ����FLASH��С
	}
	int i,Erases_num=size/PAGE_SIZE;//����������� = ������Ĵ�С / ������С +1
	if(size%PAGE_SIZE!=0)
		Erases_num++;
	for(i=0;(i<Erases_num)&&((FLASHStatus == FMC_READY));i++)
	{
		FLASHStatus = fmc_page_erase(First_address + (PAGE_SIZE * i));//��������ռ��׵�ַ��ʼ���� Erases_num ������
	}
	fmc_lock(); //����flash
	return 1;
}

/**
*@Description :��FLASHд������
*@param       :д���׵�ַ
*@param       :д�������ָ��
*@param       :д������
*@return      :д���Ƿ�ɹ�
**/
uint8_t Flash_Write(uint32_t Write_address,uint8_t *data,uint16_t Write_num)
{
	uint16_t i;
	if((Write_address<0x8000000) || Write_address>=(Flash_SIZE+0x8000000))
		return 0; //flashд��Խ�� �� flashд�볬���ڴ�
	fmc_unlock();     // ����flash
	for(i=0;i<Write_num;i+=4)
	{
		while(1)
		{
			if (*(uint32_t*)(Write_address+i) != *(uint32_t*)(data+i))       //�����Ƿ���ͬ
			{
				fmc_word_program(Write_address+i, *(uint32_t*)(data+i));			//��ͬ ����
			}else
			{
				break;                                                              //����Ҫ����
			}
		}
	}
	fmc_lock(); //����flash
	return 1;
}
/**
*@Description :��FLASH��������
*@param       :�����׵�ַ
*@param       :����������ָ��
*@param       :��������
**/
void Flash_Read(uint32_t Read_address,uint8_t *data,uint16_t Read_num)
{
	uint16_t i;
	for(i=0;i<Read_num;i++)
	{
		*(data+i) = *(uint8_t*)(Read_address+i);
	}
}








