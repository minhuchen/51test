#include "mpu6050.h"


//**************************************
//��ʼ��MPU6050
//**************************************
void InitMPU6050()
{
	SendBytes(SlaveAddress, PWR_MGMT_1, 0x00);	//�������״̬
	SendBytes(SlaveAddress, SMPLRT_DIV, 0x07);
	SendBytes(SlaveAddress, CONFIG, 0x06);
	SendBytes(SlaveAddress, GYRO_CONFIG, 0x18);
	SendBytes(SlaveAddress, ACCEL_CONFIG, 0x01);
}
//**************************************
//�ϳ�����
//**************************************
int16_t GetMPU6050Data(u8 REG_Address)
{
	u8 H,L;
	H=RecvByteFromSlave(SlaveAddress, REG_Address);
	L=RecvByteFromSlave(SlaveAddress, (u8)(REG_Address+1));
	return (H<<8)+L;   //�ϳ�����
}