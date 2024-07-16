#include "i2c.h"


/********************** I2C函数 ************************/
void I2C_init(void)
{
    P_SW2 = (P_SW2 & ~(3<<4)) | (1<<4);     //IO口切换. 0: P1.4 P1.5, 1: P2.4 P2.5, 3: P3.3 P3.2
    I2CCFG = 0xe0;              //使能I2C主机模式 speed=SYSCLK/2/(2*32 + 4)=24M/136 =0.176M 
    I2CMSST = 0x00;
}


void Wait()
{
    while (!(I2CMSST & 0x40));
    I2CMSST &= ~0x40;
}

void Start()
{
    I2CMSCR = 0x01;                         //发送START命令
    Wait();
}

void SendByte(u8 dat)
{
    I2CTXD = dat;                           //写数据到数据缓冲区
    I2CMSCR = 0x02;                         //发送SEND命令
    Wait();
}

void SendBytes(u8 slaveAddress, u8 regAddress, u8 dat)
{	
	Start();
	SendByte(slaveAddress);   //从机地址，SA0=0,RW=0 
	RecvACK();
	SendByte(regAddress);  
	RecvACK();
	SendByte(dat); 
	RecvACK();	
	Stop();
}

void RecvACK()
{
    I2CMSCR = 0x03;                         //发送读ACK命令
    Wait();
}

u8 RecvByte()
{
    I2CMSCR = 0x04;                         //发送RECV命令
    Wait();
    return I2CRXD;
}

u8 RecvByteFromSlave(u8 slaveAddress, u8 regAddress)
{
	u8 dat;
	Start();
	SendByte(slaveAddress);  
  RecvACK();
	SendByte(regAddress);  
  RecvACK();
	
	Start();
	SendByte((u8)(slaveAddress+1));
  RecvACK();
	
	dat = RecvByte();
	SendNACK(); 
	Stop();
	
	return dat;
}

//void SendACK()
//{
//    I2CMSST = 0x00;                         //设置ACK信号
//    I2CMSCR = 0x05;                         //发送ACK命令
//    Wait();
//}

void SendNACK()
{
    I2CMSST = 0x01;                         //设置NAK信号
    I2CMSCR = 0x05;                         //发送ACK命令
    Wait();
}

void Stop()
{
    I2CMSCR = 0x06;                         //发送STOP命令
    Wait();
}
