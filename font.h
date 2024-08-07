#ifndef _H_FONT_
#define _H_FONT_
#include "common.h"


/************************************6*8�ĵ���************************************/
const u8 code F6x8[][6] =		
{
0x00, 0x08, 0x08, 0x3E, 0x08, 0x08,// +
0x00, 0x08, 0x08, 0x08, 0x08, 0x08,// -
0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
0x00, 0x00, 0x42, 0x7F, 0x40, 0x00,// 1
0x00, 0x42, 0x61, 0x51, 0x49, 0x46,// 2
0x00, 0x21, 0x41, 0x45, 0x4B, 0x31,// 3
0x00, 0x18, 0x14, 0x12, 0x7F, 0x10,// 4
0x00, 0x27, 0x45, 0x45, 0x45, 0x39,// 5
0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
0x00, 0x01, 0x71, 0x09, 0x05, 0x03,// 7
0x00, 0x36, 0x49, 0x49, 0x49, 0x36,// 8
0x00, 0x06, 0x49, 0x49, 0x29, 0x1E,// 9
0x00, 0x7F, 0x09, 0x09, 0x09, 0x06,// P
0x00, 0x7F, 0x09, 0x19, 0x29, 0x46,// R
0x00, 0x63, 0x14, 0x08, 0x14, 0x63,// X
0x00, 0x07, 0x08, 0x70, 0x08, 0x07,// Y
0x00, 0x61, 0x51, 0x49, 0x45, 0x43,// Z
};


#define FONT_PLUS				0
#define FONT_MINUS			1
#define FONT_0					2
#define FONT_1					3
#define FONT_2					4
#define FONT_3					5
#define FONT_4					6
#define FONT_5					7
#define FONT_6					8
#define FONT_7					9
#define FONT_8					10
#define FONT_9					11
#define FONT_P					12
#define FONT_R					13
#define FONT_X					14
#define FONT_Y					15
#define FONT_Z					16

const u8 code POSMAP[][2] = {
		'+', FONT_PLUS,
		'-', FONT_MINUS,
		'P', FONT_P,
		'R', FONT_R,
		'X', FONT_X,
		'Y', FONT_Y,
		'Z', FONT_Z,
		0,0
};


#endif