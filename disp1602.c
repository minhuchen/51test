#include "disp1602.h"


/************* LCD1602相关程序	*****************************************************/
//8位数据访问方式	LCD1602		标准程序	梁工编写	2014-2-21, 
//justdoit updated and changed to 4 bit mode, 2024/07
#define LineLength	16		//16x2

/*
total 2 lines, 16x2= 32
first line address:  0~15
second line address: 64~79

*/

#define C_CLEAR			0x01		//clear LCD
#define C_HOME 			0x02		//cursor go home
#define C_CUR_L			0x04		//cursor shift left after input
#define C_RIGHT			0x05		//picture shift right after input
#define C_CUR_R			0x06		//cursor shift right after input
#define C_LEFT 			0x07		//picture shift left after input
#define C_OFF  			0x08		//turn off LCD
#define C_ON   			0x0C		//turn on  LCD
#define C_FLASH			0x0D		//turn on  LCD, flash 
#define C_CURSOR		0x0E		//turn on  LCD and cursor
#define C_FLASH_ALL		0x0F		//turn on  LCD and cursor, flash
#define C_CURSOR_LEFT	0x10		//single cursor shift left
#define C_CURSOR_RIGHT	0x14		//single cursor shift right
#define C_PICTURE_LEFT	0x18		//single picture shift left
#define C_PICTURE_RIGHT	0x1C		//single picture shift right
#define C_L1DOT7		0x30		//8 bits,one line 5*7  dots
#define C_L1DOT10		0x34		//8 bits,one line 5*10 dots
#define C_L2DOT7		0x38		//8 bits,tow lines 5*7 dots
#define C_4bitL2DOT7	0x28		//4 bits,tow lines 5*7 dots

#define C_CGADDRESS0	0x40		//CGRAM address0 (addr=40H+x)
#define C_DDADDRESS0	0x80		//DDRAM address0 (addr=80H+x)

void LCD_BusData(u8 Data)
{
    if(Data & 1)
    	LCD_B4 = 1;
    else
    	LCD_B4 = 0;		
    if(Data & 2)
    	LCD_B5 = 1;
    else
    	LCD_B5 = 0;
    if(Data & 4)
    	LCD_B6 = 1;
    else
    	LCD_B6 = 0;
    if(Data & 8)
    	LCD_B7 = 1;
    else
    	LCD_B7 = 0;
}



void LCD_DelayNop(void)
{
	u16	i;
	for (i=0; i<100; ++i){
		_nop_();
	}
}

void LCD_4Bits(u8 val) {	
	LCD_BusData(val);	
	LCD_ENA = 1;
	  LCD_DelayNop();
	LCD_ENA = 0;	
	  LCD_DelayNop();
}

//========================================================================
// 函数: void Write_CMD(u8 cmd)
// 描述: 写命令
// 参数: cmd: 要写的命令. 
//========================================================================
void Write_CMD(u8 cmd)
{
	LCD_RS  = 0;
	LCD_RW = 0;
	
	LCD_4Bits(cmd>>4);	
	LCD_4Bits(cmd&0xF);	
}
//========================================================================
// 函数: void Write_DIS_Data(u8 dat)
// 描述: 写显示数据(检测忙)
// 参数: dat: 要写的数据.
//========================================================================
void Write_DIS_Data(u8 dat)
{	
	LCD_RS  = 1;
	LCD_RW = 0;	
	
	LCD_4Bits(dat>>4);	
	LCD_4Bits(dat&0xF);	
}

//========================================================================
// 函数: void Initialize_LCD(void)
// 描述: 初始化函数
//========================================================================
void Initialize_LCD(void)
{
	LCD_RS  = 0;
	LCD_RW = 0;
	LCD_ENA = 0;
	LCD_BusData(0);
	delay_ms(50);	
	LCD_4Bits(3);
	delay_ms(5);	
	LCD_4Bits(3);
	delay_ms(1);	
	LCD_4Bits(3);	
	LCD_4Bits(2);	
	Write_CMD(C_4bitL2DOT7);	
	Write_CMD(C_CLEAR);		//clear LCD RAM
	delay_ms(2);
	Write_CMD(C_CUR_R);		//Curror Shift Right
	Write_CMD(C_ON);		//turn on  LCD
}

//========================================================================
// 函数: void PutString(u8 row, u8 column, u8 *puts)
// 描述: 写一个字符串，指定行、列和字符串首地址
// 参数: row: 行(0或1),  column: 第几个字符(0~15),  puts: 要写的字符串指针.
//========================================================================
void PutString(u8 row, u8 column, u8 *puts)
{
	Write_CMD((((row & 1) << 6) + column) | 0x80);
	for ( ;  *puts != 0;  puts++)		//遇到停止符0结束
	{
		Write_DIS_Data(*puts);
		if(++column >= LineLength)	break;
	}
}
