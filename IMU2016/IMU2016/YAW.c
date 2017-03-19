/*
 * GccApplication1.c
 *
 * Created: 8/12/2015 6:57:52 PM
 *  Author: Naasa Fikri
 *  
 */ 

 
#include <avr/io.h>
#include "can_lib.h"
#include "config.h"
#include "i2cmaster.h"
#include "../../car_23.h";
#include <math.h>

#define PI  (3.14159265F);

//i2c addresses
static unsigned char gyro = GYRO<<1;

void uc_init(void){
	//i2c initialization macro
	//i2c_init();	
	
	
	/*
	* CAN Bus Enable section
	* MUST SET CAN_BAUDRATE in config.h 
	*/	
	//disable can controller for initialization
	//CANGCON &= ~(1<<ENASTB);
	CANGCON  =  (1<<SWRES);
	 
	//set can timing to 250KBPS
	CANBT1 =  0x06;
	CANBT2 =  0x0C;
	CANBT3 =  0x37;
	////clear MObs
	can_clear_all_mob();
	//
	//////Enable CAN controller in normal mode
	CANGCON = (1<<ENASTB);				//! Enable CAN controller 
    while ((CANGSTA & (1<<ENFG)) == 0); //! Wait for Enable OK
    CANGIT = 0xFF;                      //! Reset General errors and OVRTIM flag
	
	
	
	/*gyro configuration
	* TODO:/ Investigate Builtin high-pass filtering
	*/
	//i2c_start_wait(gyro+I2C_WRITE);     // set device address and write mode
	//i2c_write(0x20);                        // write address CTRL_REG1	
	//i2c_write(0x00);                        // clear value at CTRL_REG1
	//i2c_stop();                        // stop i2c
	//
	//i2c_start_wait(gyro+I2C_WRITE);     // set device address and write mode
	//i2c_write(0x20);                        // write address CTRL_REG1	
	//i2c_write(0x0F);                        // normal mode
	//i2c_stop();                        // stop i2c
}

int main(void)
{		
	//initialize system
	uc_init();
	
	//data holders
	int volatile  roll =10, pitch =10, yaw =10;
	U8 gyro_buffer[6];
	
	st_cmd_t static gyro_msg;
	gyro_msg.id.std = IMU;
	gyro_msg.ctrl.ide = 0;
	gyro_msg.ctrl.rtr = 0;
	//.......
	gyro_msg.pt_data = &gyro_buffer[0];//point msg data to data buffer

	gyro_msg.cmd = CMD_TX;
	gyro_msg.dlc = 6;
	
		
	while(1){
		//-----GYRO-----
		//retrieve data from i2c gyro sensor
		//i2c_start_wait(gyro+I2C_WRITE);     // set device address and write mode
		//i2c_write(0x80 | 0x28);                        // write address 0x28 and set it to auto increment
		//i2c_rep_start(gyro+I2C_READ);			//repeated start and send the sub address
		//roll = i2c_readAck();                    // Roll LSB
		//roll |= i2c_readAck() << 8;				//Roll MSB
		//pitch = i2c_readAck();                    // Pitch LSB
		//pitch |= i2c_readAck() << 8;				//Pitch MSB
		//yaw = i2c_readAck();                    // Yaw LSB
		//yaw |= i2c_readNak() << 8;				//Yaw MSB

		
		
		//store temp, in buffer
		gyro_buffer[0] = (roll<<8)>>8;
		gyro_buffer[1] = roll>>8;	
		gyro_buffer[2] = (pitch<<8)>>8;	
		gyro_buffer[3] = pitch>>8;	
		gyro_buffer[4] = (yaw<<8)>>8;	
		gyro_buffer[5] = yaw>>8;	
		

		//send can message to MOb
		while(can_cmd(&gyro_msg)!= CAN_CMD_ACCEPTED);
		//loop until send successful 
		while(can_get_status(&gyro_msg) != CAN_STATUS_COMPLETED);	

	}
	

}
