/*
 * acl.h
 *
 *  Created on: Feb 4, 2012
 *      Author: Administrator
 */

#ifndef ACL_H_
#define ACL_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2cmaster.h"

uint8_t acl_init(void);
uint8_t acl_get_values(void);
uint8_t acl_write(uint8_t reg,uint8_t data);
uint8_t acl_read(uint8_t reg,uint8_t *data);

extern volatile int16_t acl_x;
extern volatile int16_t acl_y;
extern volatile int16_t acl_z;

#define ACL_X_OFFSET	-46L
#define ACL_X_MUL		945L
#define ACL_Y_OFFSET	-30L
#define ACL_Y_MUL		953L
#define ACL_Z_OFFSET	-19L
#define ACL_Z_MUL		980L

#define ACL_WR	0xA6
#define ACL_RD	0xA7

// ADXL345 Registers
#define ACL_DEVID 			0x00	// Device ID
#define ACL_THRESH_TAP		0x1D	// Tap threshold
#define ACL_OFSX			0x1E	// X-axis offset
#define ACL_OFSY			0x1F	// Y-axis offset
#define ACL_OFSZ			0x20	// Z-axis offset
#define ACL_DUR				0x21	// Tap duration
#define ACL_LATENT			0x22	// Tap latency
#define ACL_WINDOW			0x23	// Tap window
#define ACL_THRESH_ACT		0x24	// Activity threshold
#define ACL_THRESH_INACT	0x25	// Inactivity threshold
#define ACL_TIME_INACT		0x26	// Inactivity time
#define ACL_ACT_INACT_CTL	0x27	// Axis enable control for activity and inactivity detection
#define ACL_THRESH_FF		0x28	// Free-fall threshold
#define ACL_TIME_FF			0x29	// Free-fall time
#define ACL_TAP_AXES		0x2A	// Axis control for single tap/double tap
#define ACL_ACT_TAP_STATUS	0x2B	// Source of single tap/double tap
#define ACL_BW_RATE			0x2C	// Data rate and power mode control
#define ACL_POWER_CTL		0x2D	// Power-saving features control
#define ACL_INT_ENABLE		0x2E	// Interrupt enable control
#define ACL_INT_MAP			0x2F	// Interrupt mapping control
#define ACL_INT_SOURCE		0x30	// Source of interrupts
#define ACL_DATA_FORMAT		0x31	// Data format control
#define ACL_DATAX0			0x32	// X-Axis Data 0
#define ACL_DATAX1			0x33	// X-Axis Data 1
#define ACL_DATAY0			0x34	// Y-Axis Data 0
#define ACL_DATAY1			0x35	// Y-Axis Data 1
#define ACL_DATAZ0			0x36	// Z-Axis Data 0
#define ACL_DATAZ1			0x37	// Z-Axis Data 1
#define ACL_FIFO_CTL		0x38	// FIFO control
#define ACL_FIFO_STATUS		0x39	// FIFO status



#endif /* ACL_H_ */
