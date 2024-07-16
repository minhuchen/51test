#ifndef _COMMON_H_
#define _COMMON_H_

#include    "stc32g.h"       //������ͷ�ļ��󣬲���Ҫ�ٰ���"reg51.h"ͷ�ļ�
#include    "intrins.h"
#include <stdio.h>
//#include <stdint.h>
#include <string.h>

#define     MAIN_Fosc       24000000L   //������ʱ��

typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

typedef	char						int8_t;
typedef	unsigned char		uint8_t;
typedef	short						int16_t;
typedef	unsigned short	uint16_t;
typedef unsigned long   uint32_t;
typedef long   					int32_t;

void delay_ms(u16 ms);


#endif