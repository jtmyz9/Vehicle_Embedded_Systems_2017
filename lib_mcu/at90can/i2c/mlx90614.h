/*
 * mlx90614.h
 *
 *  Created on: Mar 30, 2012
 *      Author: Administrator
 */

#ifndef MLX90614_H_
#define MLX90614_H_

extern unsigned int mlx_read(unsigned int address, unsigned int command);
extern void mlx_write(unsigned char address, unsigned char command, unsigned int newvalue);
extern float mlx_getObjTempK(unsigned char address);
extern float mlx_getObjTempC(unsigned char address);
extern float mlx_getObjTempF(unsigned char address);
extern float mlx_getAmbTempK(unsigned char address);
extern float mlx_getAmbTempC(unsigned char address);
extern unsigned int mlx_getAddress(void);
extern int mlx_setAddress(unsigned char oldaddress, unsigned char newaddress);

#endif /* MLX90614_H_ */
