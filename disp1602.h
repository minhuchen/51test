#ifndef _DISP1602_H_
#define _DISP1602_H_

#include "common.h"

// LCD16x2 pins
#define	LCD_ENA	P00
#define	LCD_RW	P01
#define	LCD_RS	P02

#define	LCD_B4  P03
#define	LCD_B5  P04
#define	LCD_B6  P05
#define	LCD_B7  P06


void Initialize_LCD(void);
void PutString(u8 row, u8 column, u8 *puts);


#endif