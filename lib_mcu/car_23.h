/*
 * car_def.h
 *
 * Created: 1/10/2016 2:57:12 PM
 *  Author: MizzouRacing
 */ 


#ifndef CAR_DEF_H_
#define CAR_DEF_H_


/*
CAN Bus Tire Temp ids
	Four individual controllers for tire temp,
	One for each corner of car
*/
typedef enum
{
	LEFT_FRONT = 0x123000,
	RIGHT_FRONT = 0x124000,
	LEFT_REAR = 0x125000,
	RIGHT_REAR = 0x126000,
	IMU = 0x127000,
	ETC = 0x128000,
	PEDAL = 0x345600,
	THROTTLE = 0x393300,
	PDM	= 0x50
	} canbus_id;


/*
* i2c address assigned to devices in vehicle
* MLX90614, IMU(gyro, accel)
*/
typedef enum
{
	INNER = 0x0C,
	MIDDLE = 0x16,
	OUTER = 0x0B,
	BRAKE = 0x04,
	ACCEL = 0x19,
	GYRO = 0x35
	} i2c_id;

/*
* ETC error codes 
*/
typedef enum 
{
	NORMAL_OPERATION = 0,
	APPS_FAILURE,
	TPS_FAILURE,
	BSE_FAILURE
} ETC_error_codes;



#endif /* CAR_DEF_H_ */