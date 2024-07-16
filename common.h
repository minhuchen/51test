#ifndef _COMMON_H_
#define _COMMON_H_

#include    "stc32g.h"       //包含此头文件后，不需要再包含"reg51.h"头文件
#include    "intrins.h"
#include <stdio.h>
//#include <stdint.h>
#include <string.h>

#define     MAIN_Fosc       24000000L   //定义主时钟

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