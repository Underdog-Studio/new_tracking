#include "flash_hal.h"

/**
*@Description :擦除指定空间flash
*@param       :需擦除空间首地址
*@param       :需擦除的大小
*@return      :擦除是否成功
**/
uint8_t Flash_Erase(uint32_t First_address,uint32_t size)
{
	fmc_unlock();     // 解锁flash
	fmc_state_enum FLASHStatus = FMC_READY;
	if(size > Flash_SIZE-(First_address-0x8000000))
	{
		fmc_lock(); //上锁flash
		return 0;//需擦除的大小 超过FLASH大小
	}
	int i,Erases_num=size/PAGE_SIZE;//需擦除扇区数 = 需擦除的大小 / 扇区大小 +1
	if(size%PAGE_SIZE!=0)
		Erases_num++;
	for(i=0;(i<Erases_num)&&((FLASHStatus == FMC_READY));i++)
	{
		FLASHStatus = fmc_page_erase(First_address + (PAGE_SIZE * i));//以需擦除空间首地址开始擦除 Erases_num 个扇区
	}
	fmc_lock(); //上锁flash
	return 1;
}

/**
*@Description :往FLASH写入数据
*@param       :写入首地址
*@param       :写入的数据指针
*@param       :写入数量
*@return      :写入是否成功
**/
uint8_t Flash_Write(uint32_t Write_address,uint8_t *data,uint16_t Write_num)
{
	uint16_t i;
	if((Write_address<0x8000000) || Write_address>=(Flash_SIZE+0x8000000))
		return 0; //flash写入越界 与 flash写入超出内存
	fmc_unlock();     // 解锁flash
	for(i=0;i<Write_num;i+=4)
	{
		while(1)
		{
			if (*(uint32_t*)(Write_address+i) != *(uint32_t*)(data+i))       //数据是否不相同
			{
				fmc_word_program(Write_address+i, *(uint32_t*)(data+i));			//不同 更新
			}else
			{
				break;                                                              //不需要更新
			}
		}
	}
	fmc_lock(); //上锁flash
	return 1;
}
/**
*@Description :从FLASH读出数据
*@param       :读出首地址
*@param       :读出的数据指针
*@param       :读出数量
**/
void Flash_Read(uint32_t Read_address,uint8_t *data,uint16_t Read_num)
{
	uint16_t i;
	for(i=0;i<Read_num;i++)
	{
		*(data+i) = *(uint8_t*)(Read_address+i);
	}
}








