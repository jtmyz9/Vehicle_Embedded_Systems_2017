/*
 * gyro.h
 *
 *  Created on: Mar 26, 2012
 *      Author: Administrator
 */

#ifndef GYRO_H_
#define GYRO_H_

#include <avr/io.h>
#include <util/delay.h>
#include "i2cmaster.h"

//address is 0x68, <<1 for I2C READ/WRITE Bit
#define GYRO_ADDR	0xD0

#define GYRO_SCALE_NUM	1000L
#define GYRO_SCALE_DEN	14375L

#define GYRO_WHO_AM_I	0x00
#define GYRO_SMPLRT_DIV	0x15
#define GYRO_DLPF_FS	0x16
#define GYRO_INT_CFG	0x17
#define GYRO_INT_STATUS	0x1A
#define GYRO_TEMP_OUT_H	0x1B
#define GYRO_TEMP_OUT_L	0x1C
#define GYRO_XOUT_H		0x1D
#define GYRO_XOUT_L		0x1E
#define GYRO_YOUT_H		0x1F
#define GYRO_YOUT_L		0x20
#define GYRO_ZOUT_H		0x21
#define GYRO_ZOUT_L		0x22
#define GYRO_PWR_MGM	0x3E

//definitions for filter and sample rate config
#define FILTER_256_RATE_8	0x18
#define FILTER_188_RATE_1	0x19
#define FILTER_98_RATE_1	0x1A
#define FILTER_42_RATE_1	0x1B
#define FILTER_20_RATE_1	0x1C
#define FILTER_10_RATE_1	0x1D
#define FILTER_5_RATE_1		0x1E



extern int16_t gyro_x,gyro_y,gyro_z;

uint8_t gyro_init(void);
void gyro_get_values(void);
uint8_t gyro_read(uint8_t start_reg,uint8_t *buf,uint8_t num);
uint8_t gyro_write(uint8_t reg,uint8_t val);
#endif /* GYRO_H_ */
