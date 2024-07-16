#ifndef _I2C_HEADER_
#define _I2C_HEADER_

#include "common.h"

void I2C_init(void);

void Wait();
void Start();
void SendByte(u8 dat);
void SendBytes(u8 slaveAddress, u8 regAddress, u8 dat);
void RecvACK();
u8 RecvByte();
u8 RecvByteFromSlave(u8 slaveAddress, u8 regAddress);
//void SendACK();
void SendNACK();
void Stop();








#endif