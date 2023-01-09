#ifndef __Button_drv_H
#define __Button_drv_H

#include "Configuration_hal.h"







void Button_init(void);



bool Get_Button_Click(void);
bool Get_Button_Double_Click(void);
bool Get_Button_Long_Press(void);






#endif