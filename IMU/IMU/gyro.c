/*
 * gyro.c
 *
 *  Created on: Mar 26, 2012
 *      Author: Administrator
 */

#include "gyro.h"
#include <util/atomic.h>

int16_t gyro_x = 0;
int16_t gyro_y = 0;
int16_t gyro_z = 0;

uint8_t gyro_init(void)
{	

	gyro_write(GYRO_PWR_MGM, 0x80);		// reset gyro for init
	gyro_write(GYRO_SMPLRT_DIV, 0x00);		// set sample rate divider to 0
	gyro_write(GYRO_INT_CFG, 0x00);		// set interrupt configuration

	/*
	*	Set Digital Low Pass Filter and Sample Rate
	*	DLPF and Sample rate must be set for proper operation
	*/
	if(gyro_write(GYRO_DLPF_FS,FILTER_20_RATE_1))
	{
		return 1;
	}

	//if unable to write sample rate, init fail
	return 0;
}

/*
* Function to Write data to Gyro
* Reg: register address to be written to 
* Val: Value to be written to register
*/
uint8_t gyro_write(uint8_t reg,uint8_t val)
{	
	//Write sequence start+write, Register address,data, stop

	if(i2c_start(GYRO_ADDR+I2C_WRITE))
	{
		i2c_stop();
		return 1;
	}

	i2c_write(reg);
	i2c_write(val);
	i2c_stop();

	return 0;
}

/*
* Read information from Gyro
* start_reg: register address to start read from
* buf: pointer to buffer to store data 
* num: number of bytes to read
* Return: 0 on success
*/
uint8_t gyro_read(uint8_t start_reg,uint8_t *buf,uint8_t num)
{
	uint8_t i;

	if(i2c_start(GYRO_ADDR+I2C_WRITE))
	{
		return 1;
	}

	i2c_write(start_reg);
	if(i2c_rep_start(GYRO_ADDR+I2C_READ))
	{
		i2c_stop();
		return 2;
	}
	for(i = 0;i < num-1;i++)
	{
		buf[i] = i2c_read(ACK);
	}

	buf[num-1] = i2c_read(NACK);

	i2c_stop();

	return 0;
}

/*
* Retrieve Values from Gyro
* Scales value to degrees/sec, 14.375 degrees/sec per LSB
* Input: Void
* Return: Void
*/
void gyro_get_values(void)
{
	uint8_t data[7];
	int32_t x,y,z;
	
#ifdef DEBUG
	data[0] = 0xff;
	data[1] = 0xff;
	data[2] = 0xff;
	data[3] = 0xff;
	data[4] = 0xff;
	data[5] = 0xff;
	data[6] = 0xff;
#endif

	gyro_read(GYRO_XOUT_H,&data,7);

	x = ((((int16_t)(data[0])) << 8) &0xFF00) + (((int16_t)data[1]) & 0x00FF);
	y = ((((int16_t)(data[2])) << 8) &0xFF00) + (((int16_t)data[3]) & 0x00FF);
	z = ((((int16_t)(data[4])) << 8) &0xFF00) + (((int16_t)data[5]) & 0x00FF);
	
	
	//x = (GYRO_SCALE_NUM*((int32_t)(x)))/GYRO_SCALE_DEN;
	//y = (GYRO_SCALE_NUM*((int32_t)(y)))/GYRO_SCALE_DEN;
	//z = (GYRO_SCALE_NUM*((int32_t)(z)))/GYRO_SCALE_DEN;

	// Guarantee atomic write
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{	
		gyro_x = (int16_t)x;
		gyro_y = (int16_t)y;
		gyro_z = (int16_t)z;

	}

}








