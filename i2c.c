#include "i2c.h"


/********************** I2C���� ************************/
void I2C_init(void)
{
    P_SW2 = (P_SW2 & ~(3<<4)) | (1<<4);     //IO���л�. 0: P1.4 P1.5, 1: P2.4 P2.5, 3: P3.3 P3.2
    I2CCFG = 0xe0;              //ʹ��I2C����ģʽ speed=SYSCLK/2/(2*32 + 4)=24M/136 =0.176M 
    I2CMSST = 0x00;
}


void Wait()
{
    while (!(I2CMSST & 0x40));
    I2CMSST &= ~0x40;
}

void Start()
{
    I2CMSCR = 0x01;                         //����START����
    Wait();
}

void SendByte(u8 dat)
{
    I2CTXD = dat;                           //д���ݵ����ݻ�����
    I2CMSCR = 0x02;                         //����SEND����
    Wait();
}

void SendBytes(u8 slaveAddress, u8 regAddress, u8 dat)
{	
	Start();
	SendByte(slaveAddress);   //�ӻ���ַ��SA0=0,RW=0 
	RecvACK();
	SendByte(regAddress);  
	RecvACK();
	SendByte(dat); 
	RecvACK();	
	Stop();
}

void RecvACK()
{
    I2CMSCR = 0x03;                         //���Ͷ�ACK����
    Wait();
}

u8 RecvByte()
{
    I2CMSCR = 0x04;                         //����RECV����
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
//    I2CMSST = 0x00;                         //����ACK�ź�
//    I2CMSCR = 0x05;                         //����ACK����
//    Wait();
//}

void SendNACK()
{
    I2CMSST = 0x01;                         //����NAK�ź�
    I2CMSCR = 0x05;                         //����ACK����
    Wait();
}

void Stop()
{
    I2CMSCR = 0x06;                         //����STOP����
    Wait();
}
