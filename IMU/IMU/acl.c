/*
 * acl.c
 *
 *  Created on: Feb 4, 2012
 *      Author: Administrator
 */

#include "acl.h"
#include <util/atomic.h>

volatile int16_t acl_x = 0;
volatile int16_t acl_y = 0;
volatile int16_t acl_z = 0;

uint8_t acl_write(uint8_t reg,uint8_t data)
{
	// Set Sampling Rate
	if(i2c_start(ACL_WR))
	{
		i2c_stop();
		return 1;
	}

	i2c_write(reg);
	i2c_write(data);
	i2c_stop();

	return 0;
}

uint8_t acl_read(uint8_t reg,uint8_t *data)
{
	// Send Register
	if(i2c_start(ACL_WR))
	{
		i2c_stop();
		return 1;
	}

	i2c_write(reg);

	// Read Register
	if(i2c_rep_start(ACL_RD))
	{
		i2c_stop();
		return 2;
	}

	*data = i2c_read(NACK);

	return 0;
}

uint8_t acl_init(void)
{
	// Set Sampling Rate
	if(acl_write(ACL_BW_RATE,0x0C)) 	return 1;

	// Configure data format
	if(acl_write(ACL_DATA_FORMAT,0x00)) return 2;

	// Configure Power Control
	if(acl_write(ACL_POWER_CTL,0x08))	return 3;

	return 0;
}

uint8_t acl_get_values(void)
{
	uint8_t x0,x1,y0,y1,z0,z1;
	int16_t x,y,z;

	//Write Address
	if(i2c_start(ACL_WR))
	{
		i2c_stop();
		return 1;
	}

	// Write register address
	i2c_write(ACL_DATAX0);

	// Repeat Start
	if(i2c_rep_start(ACL_RD))
	{
		i2c_stop();
		return 2;
	}

	// Read data
	x0 = i2c_read(ACK);
	x1 = i2c_read(ACK);
	y0 = i2c_read(ACK);
	y1 = i2c_read(ACK);
	z0 = i2c_read(ACK);
	z1 = i2c_read(NACK);

	// I2c stop
	i2c_stop();

	// Parse into signed 16-bit integer
	x = 4*((((int16_t)x1) << 8) + (int16_t)x0);
	y = 4*((((int16_t)y1) << 8) + (int16_t)y0);
	z = 4*((((int16_t)z1) << 8) + (int16_t)z0);

	// Compensation (MUL*(val+OFFSET)/1000)
	x = (uint16_t)((ACL_X_MUL*((int32_t)x + ACL_X_OFFSET))/1000);
	y = (uint16_t)((ACL_Y_MUL*((int32_t)y + ACL_Y_OFFSET))/1000);
	z = (uint16_t)((ACL_Z_MUL*((int32_t)z + ACL_Z_OFFSET))/1000);

	// Guarantee atomic write
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		acl_x = x;
		acl_y = y;
		acl_z = z;
	}

	return 0;
}
