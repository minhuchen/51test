#ifndef _DISP112864_H_
#define _DISP112864_H_

#include "i2c.h"


void OLED_init(void);
//void OLED_Write_Command(u8 dat);
void SetOLED_PageMode(bit pageMode);
void SendPic();
void OLED_Clear(void);
//void OLED_On(void);
void OLED_ShowChar(u8 row, u8 column,u8 chr);
void OLED_ShowString(u8 row, u8 column, u8 *puts);




#endif