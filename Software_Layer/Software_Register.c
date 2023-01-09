#include "Software_Register.h"

/**
*@Description :����Ĵ�����ʼ��
*@param       :����Ĵ���ָ��
**/
void Software_Register_Init(Software_Register *Register)
{
	uint8_t i;
	for(i=0;i<Register->Register_Len;i++)
	{
		Register->Permission[i] = Read_Write;
		Register->Register[i] = 0;
	}
}
/**
*@Description :����Ĵ���������
*@param       :����Ĵ���ָ��
*@param       :�������Ĵ�����ַ
*@param       :����������ָ��
*@return      :����״̬
**/
bool Software_Register_Read(Software_Register *Register,uint8_t addr,uint8_t *data)
{
	if(Register->Register_Len<=addr)
		return false;
	if(Register->Permission[addr] <= Read_Write)
	{
		*data =  Register->Register[addr];
		return true;
	}
	return false;
}
/**
*@Description :����Ĵ���д����
*@param       :����Ĵ���ָ��
*@param       :�������Ĵ�����ַ
*@param       :��д������ָ��
*@return      :����״̬
**/
bool Software_Register_Write(Software_Register *Register,uint8_t addr,uint8_t *data)
{
	if(Register->Register_Len<=addr)
		return false;
	if(Register->Permission[addr] >= Read_Write)
	{
		Register->Register[addr] = *data;
		return true;
	}
	return false;
}
/**
*@Description :����Ĵ�������������
*@param       :����Ĵ���ָ��
*@param       :�������Ĵ�����ַ
*@param       :����������ָ��
*@param       :������������
*@return      :�ɹ���������
**/
uint8_t Software_Register_ReadLen(Software_Register *Register,uint8_t addr,uint8_t *data,uint8_t Len)
{
	uint8_t temp = 0;
	if(Register->Register_Len<=(addr+Len))
		return 0;
	for(;temp<Len;)
	{
		if(Register->Permission[temp+addr] <= Read_Write)
		{
			data[temp] = Register->Register[temp+addr];
			temp++;
		}else
		{
			break;
		}
	}
	return temp;
}
/**
*@Description :����Ĵ�������д����
*@param       :����Ĵ���ָ��
*@param       :�������Ĵ�����ַ
*@param       :��д������ָ��
*@param       :��д��������
*@return      :�ɹ�д������
**/
uint8_t Software_Register_WriteLen(Software_Register *Register,uint8_t addr,uint8_t *data,uint8_t Len)
{
	uint8_t temp = 0;
	if(Register->Register_Len<=(addr+Len))
		return 0;
	for(;temp<Len;)
	{
		if(Register->Permission[temp+addr] >= Read_Write)
		{
			Register->Register[temp+addr] = data[temp];
			temp++;
		}else
		{
			break;
		}
	}
	return temp;
}






