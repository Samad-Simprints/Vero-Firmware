/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __I2C_H_INCLUDED__
#define __I2C_H_INCLUDED__

void i2c_init(uint16_t sclPort, uint16_t sclPin, uint16_t sdaPort, uint16_t sdaPin);
void i2c_sendStart(void);
void i2c_sendStop(void);
unsigned char i2c_writeByte(unsigned char sendStart, unsigned char sendStop, unsigned char byte);
unsigned char i2c_readByte(unsigned char nack, unsigned char sendStop);

#endif // __I2C_H_INCLUDED__
