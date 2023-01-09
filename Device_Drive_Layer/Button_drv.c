#include "Button_drv.h"


bool Button_Click = false,Button_Double_Click = false,Button_Long_Press = false; //布尔 先定义四种模式定义为假
bool Button_State = false;
Time Button_T;

uint8_t Button_Count=0;

/**
*@Description :按键中断回调
**/
static void ButtonA_Callback(void)
{
	float Deta_T=Get_deta_T(Button_T);
	if(Button_State==false)
	{
		if(Deta_T<0.3f)
			Button_Count++;
		else
			Button_Count = 0;
		Button_T = Get_System_T();
		Button_State=true;
		Set_EXTI_TRIG(1,0);
	}else
	{
		if(Deta_T<0.3f)
		{
			Button_Count++;
			if(Button_Count == 1)//单击
			{
				Button_Click = true;
				Button_Count = 0;
			}else if(Button_Count == 2 && Button_Click == true)//双击
			{
				Button_Click = false;
				Button_Double_Click = true;
				Button_Count = 0;
			}
		}else
			Button_Count = 0;
		if(Deta_T>0.5f)//长按
		{
			Button_Long_Press=true;
			Button_Count = 0;
		}
		Button_T = Get_System_T();
		Button_State = false;
		Set_EXTI_TRIG(1,1);
	}
}




void Button_init(void)
{
	Set_EXTI_TRIG(1,1);
	EXTI1_Callback_Register(ButtonA_Callback);//注册中断回调
	Button_T = Get_System_T();
}

/**
*@Description :获取按键单击
*@return      :true 产生单击 false 未产生
**/
bool Get_Button_Click(void)
{
	if(Get_deta_T(Button_T)>0.2f)
	{
		bool temp = Button_Click;
		Button_Click = false;
		return temp;
	}
	return false;
}

/**
*@Description :获取按键双击
*@return      :true 产生双击 false 未产生
**/
bool Get_Button_Double_Click(void)
{
	bool temp = Button_Double_Click;
	Button_Double_Click = false;
	return temp;
}

/**
*@Description :获取按键长按
*@return      :true 产生长按 false 未产生
**/
bool Get_Button_Long_Press(void)
{
	bool temp = Button_Long_Press;
	Button_Long_Press = false;
	return temp;
}


	









