#include "disp1602.h"
#include "disp128_64.h"
#include "mpu6050.h"

// ʹ��Keil C251��������Memory Model�Ƽ�����XSmallģʽ��Ĭ�϶��������edata����ʱ�Ӵ�ȡ�����ٶȿ졣
// edata���鱣��1K����ջʹ�ã��ռ䲻��ʱ�ɽ������顢�����ñ�����xdata�ؼ��ֶ��嵽xdata�ռ䡣
// 
// Note: this program was based on [STC ����] and [����public open��Դ];
// 
// This test project was created by justdoit in 2024/07

u8 vals[8];

void toVals(int16_t dat)
{
	u8 idx = 6;
	if(dat<0) {
		dat=-dat;
		vals[1]='-';
	} else {
		vals[1]='+';
	}
	while(idx>1) {
		vals[idx--] =dat%10+0x30; 	
		dat/=10;
	}
}



void main(void)
{
    WTST = 0;  //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    EAXFR = 1; //��չ�Ĵ���(XFR)����ʹ��
    CKCON = 0; //��߷���XRAM�ٶ�       

    P0M1 = 0x00;   P0M0 = 0x00;   //����Ϊ׼˫���
    P1M1 = 0x00;   P1M0 = 0x00;   //����Ϊ׼˫���
    P2M1 = 0x00;   P2M0 = 0x00;   //����Ϊ׼˫���
    P3M1 = 0x00;   P3M0 = 0x00;   //����Ϊ׼˫���
    P4M1 = 0x00;   P4M0 = 0x00;   //����Ϊ׼˫���
    P5M1 = 0x00;   P5M0 = 0x00;   //����Ϊ׼˫���	
	
		vals[7] = 0;
		
		// lcd1602
    Initialize_LCD(); 

		PutString(0, 0, "Hello,  there");
		PutString(1, 0, "Just do it...");
		
		// demo OLED 128-64
    I2C_init();
    EA = 1;     //�������ж�
    OLED_init();	
		OLED_Clear();
		SetOLED_PageMode(0);
		SendPic(); //��ʾͼƬ
		
		
		delay_ms(500);
		SetOLED_PageMode(1);
		
		// mpu6050
		InitMPU6050();
		
    while(1) {
        delay_ms(500);
			
				toVals(GetMPU6050Data(ACCEL_XOUT_H));
				vals[0] = 'X';
				PutString(0, 0, vals);//��ʾX����ٶ�
				OLED_ShowString(3, 0, vals);
				
			
				vals[0] = 'Y';
				toVals(GetMPU6050Data(ACCEL_YOUT_H));
				PutString(0, 8, vals);//��ʾY����ٶ�	
				OLED_ShowString(3, 60, vals);		
			
				vals[0] = 'Z';
				toVals(GetMPU6050Data(ACCEL_ZOUT_H));
				//PutString(0, 10, vals);//��ʾY����ٶ�
				OLED_ShowString(4, 0, vals);		
			
				vals[0] = 'R'; // roll
				toVals(GetMPU6050Data(GYRO_XOUT_H));
				PutString(1, 0, vals);//��ʾX����ٶ�
				OLED_ShowString(6, 0, vals);		
			
				vals[0] = 'P'; // pitch
				toVals(GetMPU6050Data(GYRO_YOUT_H));
				PutString(1, 8, vals);//��ʾY����ٶ�
				OLED_ShowString(6, 60, vals);		
			
				vals[0] = 'Y'; // yaw
				toVals(GetMPU6050Data(GYRO_ZOUT_H));
				//PutString(1, 10, vals);//��ʾZ����ٶ�
				OLED_ShowString(7, 0, vals);		
				
    }
}
