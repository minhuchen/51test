#include "disp128_64.h"
#include "font.h"
#include "pic.h"

// Note: this file was based on [STC 例程] and [网络public open资源];
// modified/improved by justdoit in 2024/07


/****************************** 用户定义宏 ***********************************/
#define SLAW            0x78
#define WRITE_CMD       0x00
#define WRITE_DATA      0x40

/*************  本地变量声明    **************/
//u8 xdata DmaTxBuffer[128*8+2];
bit	DmaTxFlag=0;

//========================================================================
// 函数: void DMA_Config(void)
// 描述: I2C DMA 功能配置.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2021-5-6
//========================================================================
//void DMA_Config(void)
//{
//	DMA_I2CT_STA = 0x00;
//	DMA_I2CT_CFG = 0x80;	//bit7 1:Enable Interrupt
//	//DMA_I2CT_AMT = 0x00;	//设置传输总字节数(低8位)：n+1
//	//DMA_I2CT_AMTH = 0x04;	//设置传输总字节数(高8位)：n+1
//	DMA_I2CT_TXAH = (u8)((u16)&DmaTxBuffer >> 8);	//I2C发送数据存储地址
//	DMA_I2CT_TXAL = (u8)((u16)&DmaTxBuffer);
//	DMA_I2CT_CR = 0x80;		//bit7 1:使能 I2CT_DMA, bit6 1:开始 I2CT_DMA

//	//DMA_I2C_ST1 = 0x00;		//设置需要传输字节数(低8位)：n+1
//	//DMA_I2C_ST2 = 0x04;		//设置需要传输字节数(高8位)：n+1
//}



/*  写命令
    Write Command   */
void OLED_Write_Command(u8 dat)
{
	SendBytes(SLAW, WRITE_CMD, dat);
}

/*  写数据
    Write Data  */
void OLED_Write_Data(u8 dat)
{
	SendBytes(SLAW, WRITE_DATA, dat);
}

void SetOLED_PageMode(bit pageMode) {	
		OLED_Write_Command(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
		OLED_Write_Command(pageMode ? 0x02: 0x00); //--0x00: Horizontal addressing mode, 0x01: Vertical addressing mode, 0x02: Page addressing mode
}

void SendPic()
{
	u8 *pData=OLED12864_IMG;
	u16 number = IMG_BYTE_SIZE-2;
	
	DMA_I2CT_STA = 0x00;
	DMA_I2CT_CFG = 0x80;	//bit7 1:Enable Interrupt
	DMA_I2CT_TXAH = (u8)((u16)pData >> 8);	//I2C发送数据存储地址
	DMA_I2CT_TXAL = (u8)((u16)pData);
	DMA_I2CT_CR = 0x80;		//bit7 1:使能 I2CT_DMA, bit6 1:开始 I2CT_DMA	
	
	
    while(I2CMSST & 0x80);    //检查I2C控制器忙碌状态    
	
    DmaTxFlag = 1;
    
    I2CMSST = 0x00;
    I2CMSCR = 0x89;         //set cmd
    DMA_I2C_CR = 0x01;
    DMA_I2CT_AMT = (u8)(number+1);  //设置传输总字节数(低8位)：number + 设备地址 + 命令/数据指令
    DMA_I2CT_AMTH = (u8)((number+1)>>8);   //设置传输总字节数(高8位)：n+1
    DMA_I2C_ST1 = (u8)(number+1);   //设置需要传输字节数(低8位)：number + 设备地址 + 命令/数据指令
    DMA_I2C_ST2 = (u8)((number+1)>>8);     //设置需要传输字节数(高8位)：n+1
    DMA_I2CT_CR |= 0x40;    //bit7 1:使能 I2CT_DMA, bit6 1:开始 I2CT_DMA

    while(DmaTxFlag);       //DMA忙检测
    DMA_I2C_CR = 0x00;
}

//========================================================================
// 函数: void I2C_DMA_Interrupt (void) interrupt 60/61
// 描述: I2C DMA中断函数
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2021-5-8
// 备注: 
//========================================================================
void I2C_DMA_Interrupt(void) interrupt 13
{
	if(DMA_I2CT_STA & 0x01)   //发送完成
	{
		DMA_I2CT_STA &= ~0x01;  //清除标志位
		DmaTxFlag = 0;
	}
	if(DMA_I2CT_STA & 0x04)   //数据覆盖
	{
		DMA_I2CT_STA &= ~0x04;  //清除标志位
	}
	
	if(DMA_I2CR_STA & 0x01)   //接收完成
	{
		DMA_I2CR_STA &= ~0x01;  //清除标志位
	}
	if(DMA_I2CR_STA & 0x02)   //数据丢弃
	{
		DMA_I2CR_STA &= ~0x02;  //清除标志位
	}
}

//========================================================================
// 函数: void I2C_Interrupt (void) interrupt 24
// 描述: I2C 中断函数
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2022-3-18
// 备注: 
//========================================================================
void I2C_Interrupt() interrupt 24
{
	I2CMSST &= ~0x40;       //I2C指令发送完成状态清除

	if(DMA_I2C_CR & 0x04)   //ACKERR
	{
		DMA_I2C_CR &= ~0x04;  //发数据后收到NAK
	}
}

void OLED_init(void)
{
    OLED_Write_Command(0xAE); //--Turn off oled panel

    /*  
        以下三条命令是给页面模式用的，行模式和列模式不要设置，会引起问题
        行模式或列模式，要用0x21和0x22设置范围
        Following three commands are for Page Addressing Mode. Do not set them in Horizontal addressing mode or Vertical addressing mode, will rise problem.
        For Horizontal addressing mode or Vertical addressing mode, should use 0x21 and 0x22 set column and page address
    */
    //	OLED_Write_Command(0x00); //-Set Lower Column Start Address for Page Addressing Mode
    //	OLED_Write_Command(0x10); //-Set Higher Column Start Address for Page Addressing Mode
    //	OLED_Write_Command(0xB0); //-Set the page start address of the target display location by command for Page Addressing Mode

    //OLED_Write_Command(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
    //OLED_Write_Command(0x00); //--0x00: Horizontal addressing mode, 0x01: Vertical addressing mode, 0x02: Page addressing mode
    
    OLED_Write_Command(0x21); //-Set Column Address
    OLED_Write_Command(0x00); //--Start address
    OLED_Write_Command(0x7f); //--End address

    OLED_Write_Command(0x22); //-Set Page Address
    OLED_Write_Command(0x00); //---Start address
    OLED_Write_Command(0x07); //--End address

    OLED_Write_Command(0x40); //-Set Display Start Line (0x40h~0x7F)

    OLED_Write_Command(0x81); //-Set Contrast Control for BANK0
    OLED_Write_Command(0x80); //    -0x00 to 0xFF, The segment output current increases as the contrast step value increases

    OLED_Write_Command(0xA1); //-Set Segment Re-map. 0xA1: Normal, 0xA0: Re-map left and right
    OLED_Write_Command(0xC8); //-Set COM Output Scan Direction. 0xC8: Normal, 0xC0: Re-map up and down
    OLED_Write_Command(0xA6); //-Set Normal/Inverse Display, 0xA6:Normal, 0xA7: Inverse

    OLED_Write_Command(0xA8); //-Set Multiplex Ratio (16~63)
    OLED_Write_Command(0x3F); //--63 multiplex mode

    OLED_Write_Command(0xD3); //-Set Display Offset (0x00~0x3F)
    OLED_Write_Command(0x00); //--No offset

    OLED_Write_Command(0xD5); //-Set display clock divide ratio/oscillator frequency
    OLED_Write_Command(0x20); //--Set Clock as 60 Frames/Sec

    OLED_Write_Command(0xD9); //-Set pre-charge period
    OLED_Write_Command(0xF2); //--Set Pre-Charge as 15 Clocks & Discharge as 2 Clock

    OLED_Write_Command(0xDA); //-Set com pins hardware configuration
    OLED_Write_Command(0x12); //

    OLED_Write_Command(0xDB); //-Set VCOM Deselect Level
    OLED_Write_Command(0x30); //--0.83xVcc

    OLED_Write_Command(0x8D); //-Set Charge Pump enable/disable
    OLED_Write_Command(0x14); //--0x14: Enable, 0x10: Disable

    OLED_Write_Command(0xA4); //-Entire Display ON, 0xA4: Disable, 0xA5: Enable
    OLED_Write_Command(0xAF); //-Turn on oled panel
}

//坐标设置
void OLED_Set_Pos(u8 x, u8 y) 
{ 	
		OLED_Write_Command(0xB0|(y&0xF));
		OLED_Write_Command(((x&0xf0)>>4)|0x10);
		OLED_Write_Command((x&0x0f)); 
}   	  
//开启OLED显示    
//void OLED_Display_On(void)
//{
//		OLED_Write_Command(0X8D);  //SET DCDC命令
//		OLED_Write_Command(0X14);  //DCDC ON
//		OLED_Write_Command(0XAF);  //DISPLAY ON
//}
////关闭OLED显示     
//void OLED_Display_Off(void)
//{
//		OLED_Write_Command(0X8D);  //SET DCDC命令
//		OLED_Write_Command(0X10);  //DCDC OFF
//		OLED_Write_Command(0XAE);  //DISPLAY OFF
//}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
		u8 i,n;		    
		for(i=0;i<8;i++)  
		{  
			OLED_Write_Command(0xB0|i);    //设置页地址（0~7）
			OLED_Write_Command(0x00);      //设置显示位置―列低地址
			OLED_Write_Command(0x10);      //设置显示位置―列高地址   
			for(n=0;n<128;n++)OLED_Write_Data(0); 
		} //更新显示
}
//void OLED_On(void)  
//{  
//		u8 i,n;		    
//		for(i=0;i<8;i++)  
//		{  
//				OLED_Write_Command(0xB0|i);    //设置页地址（0~7）
//				OLED_Write_Command(0x00);      //设置显示位置―列低地址
//				OLED_Write_Command(0x10);      //设置显示位置―列高地址   
//				for(n=0;n<128;n++)OLED_Write_Data(0xFF); 
//		} //更新显示
//}

//在指定位置显示一个字符,包括部分字符
void OLED_ShowChar(u8 row, u8 column,u8 chr)
{      	
		u8 i=0, pos, ch;	
		//得到偏移后的值	
		if (chr>='0' && chr<='9')pos = chr-'0'+FONT_0;
		else {
			while(ch=POSMAP[i][0]) {
				if (chr == ch) {
					pos=POSMAP[i][1];
					break;
				}					
				++i;
			}
		}
	
	
		OLED_Set_Pos(column,row);
		for(i=0;i<6;i++) {
			OLED_Write_Data(F6x8[pos][i]);
		}
}

void OLED_ShowString(u8 row, u8 column, u8 *puts) {
		u8 chr;
		while(chr=*puts++) {
			OLED_ShowChar(row, column, chr);
			column+=6;
		}
}