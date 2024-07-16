#include "disp1602.h"
#include "disp128_64.h"
#include "mpu6050.h"

// 使用Keil C251编译器，Memory Model推荐设置XSmall模式，默认定义变量在edata，单时钟存取访问速度快。
// edata建议保留1K给堆栈使用，空间不够时可将大数组、不常用变量加xdata关键字定义到xdata空间。
// 
// Note: this program was based on [STC 例程] and [网络public open资源];
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
    WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    EAXFR = 1; //扩展寄存器(XFR)访问使能
    CKCON = 0; //提高访问XRAM速度       

    P0M1 = 0x00;   P0M0 = 0x00;   //设置为准双向口
    P1M1 = 0x00;   P1M0 = 0x00;   //设置为准双向口
    P2M1 = 0x00;   P2M0 = 0x00;   //设置为准双向口
    P3M1 = 0x00;   P3M0 = 0x00;   //设置为准双向口
    P4M1 = 0x00;   P4M0 = 0x00;   //设置为准双向口
    P5M1 = 0x00;   P5M0 = 0x00;   //设置为准双向口	
	
		vals[7] = 0;
		
		// lcd1602
    Initialize_LCD(); 

		PutString(0, 0, "Hello,  there");
		PutString(1, 0, "Just do it...");
		
		// demo OLED 128-64
    I2C_init();
    EA = 1;     //允许总中断
    OLED_init();	
		OLED_Clear();
		SetOLED_PageMode(0);
		SendPic(); //显示图片
		
		
		delay_ms(500);
		SetOLED_PageMode(1);
		
		// mpu6050
		InitMPU6050();
		
    while(1) {
        delay_ms(500);
			
				toVals(GetMPU6050Data(ACCEL_XOUT_H));
				vals[0] = 'X';
				PutString(0, 0, vals);//显示X轴加速度
				OLED_ShowString(3, 0, vals);
				
			
				vals[0] = 'Y';
				toVals(GetMPU6050Data(ACCEL_YOUT_H));
				PutString(0, 8, vals);//显示Y轴加速度	
				OLED_ShowString(3, 60, vals);		
			
				vals[0] = 'Z';
				toVals(GetMPU6050Data(ACCEL_ZOUT_H));
				//PutString(0, 10, vals);//显示Y轴加速度
				OLED_ShowString(4, 0, vals);		
			
				vals[0] = 'R'; // roll
				toVals(GetMPU6050Data(GYRO_XOUT_H));
				PutString(1, 0, vals);//显示X轴角速度
				OLED_ShowString(6, 0, vals);		
			
				vals[0] = 'P'; // pitch
				toVals(GetMPU6050Data(GYRO_YOUT_H));
				PutString(1, 8, vals);//显示Y轴角速度
				OLED_ShowString(6, 60, vals);		
			
				vals[0] = 'Y'; // yaw
				toVals(GetMPU6050Data(GYRO_ZOUT_H));
				//PutString(1, 10, vals);//显示Z轴角速度
				OLED_ShowString(7, 0, vals);		
				
    }
}
