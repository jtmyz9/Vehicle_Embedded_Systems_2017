/*
 * IMU.c
 *	Original Author : Mark Shaumburg
 *  	Created on: Feb 4, 2012
 *  Revision 2: Jason McLoud
 * 		1/2/2017
 * 		Notes: Removed Suspesion Travel(Potetionmeter), utilized Atmel Libraries(CAN, UART)
 *	TODO: UART Lib, Mark's Library works but no comments, Atmel lib is clean af
 *
 *	 Send 3-Axis Accel and 3-Axis Gyro Data 
 *			 Data sent through CAN at predetermined frequency 
 *			 Datalogging System recieves and logs
 */


#include "IMU.h"

int mcu_init(){
	Disable_interrupt();
	//Set DDR for port D and E for LEDs
	DDRD|= (1<<PD7);
	DDRE|= (1<<PE6);
	
		

#ifdef DEBUG
	stdout = stdin = &uart_str;
	uart_init(CONF_8BIT_NOPAR_1STOP, UART_BAUDRATE);
	//uart only utilized for usb/serial debugging purposes
	uint8_t resp;
#endif

	//CAN Init section
	Can_reset();
	can_init(1);
	while ((CANGSTA & (1<<ENFG)) == 0); //! Wait for Enable OK
	CANGIT = 0xFF;                      //! Reset General errors and OVRTIM flag
	can_clear_all_mob();				//Clear all Mobs

	//InitCanMsgs();
	imu.id.std = IMU;
	imu.pt_data = &data[0];//point msg data to data buffer
	imu.cmd = CMD_TX;
	imu.dlc = 6;


i2c_init();

#ifdef DEBUG
	printf("Start\n");

//	if((resp = acl_init())!= 0)	printf("ACL initialization error: %u\n",resp);
//	_delay_us(1000);
	if((resp = gyro_init())!= 0) printf("Gyro initialization error: %u\n", resp);
	else printf("gyro Init ok\n");
	gyro_x = 0xCA;
	gyro_y = 0xCA;
	gyro_z = 0xCA;	
	_delay_us(100);
	
#else
	acl_init();
	_delay_us(100);
	gyro_init();
	_delay_us(100);
#endif
 
	

	systimer_10ms_restart(10);
	
	return 0;
}

int main(void)
{	
	mcu_init();
	int i = 0;
	
	Enable_interrupt();
	while(1)
	{
		//	acl_get_values();
		//	_delay_us(100);
			gyro_get_values();
		i++;
			_delay_us(100);
	}
}

/*
*	Called by timer compare match ISR
*	Compare Vector set to trigger on 10ms intervals
*	handle_10ms_tick: handle for sending messages to can controller
*	USAGE: set data output frequency, based upon current systimer val
*/
void handle_10ms_tick(void)
{	
	//send message for gyro and accel	
	if(systimer_10ms %10 == 0)
	{

#ifndef DEBUG
		data[0]=(gyro_x >> 8) & 0x00FF;
		data[1]=(gyro_x & 0x00FF);
		data[2]=(gyro_y >> 8) & 0x00FF;
		data[3]=(gyro_y & 0x00FF);
		data[4]=(gyro_z >> 8) & 0x00FF;
		data[5]=(gyro_z & 0x00FF);

		can_clear_all_mob();
		//send can message to MOb
		while(can_cmd(&imu)!= CAN_CMD_ACCEPTED);
		//loop until send successful
		while(can_get_status(&imu) != CAN_STATUS_COMPLETED){
		}
#endif

		
	}

	if(systimer_10ms %20 == 0)
	{
		// Send 50Hz Data
		//led_ctrl(LED2,LED_ON,LED_NO_TMR);
	}

	if(systimer_10ms % 50 == 0)
	{
		// Send 20Hz Data
		//led_ctrl(LED3,LED_ON,LED_NO_TMR);
	}

	if(systimer_10ms % 100 == 0)
	{
		// Send 10Hz Data
		

	}

	if(systimer_10ms % 200 == 0)
	{
		// Send 5Hz Data
		//led_ctrl(LED5,LED_ON,LED_NO_TMR);
	}
	if(systimer_10ms % 500 == 0)
	{
		// Send 2Hz Data
		//led_ctrl(LED6,LED_ON,LED_NO_TMR);
	}
	if(systimer_10ms % 1000 == 0)
	{
		// Send 1Hz Data
		//led_ctrl(LED7,LED_ON,LED_NO_TMR);
#ifdef DEBUG
		data[0]=(gyro_x >> 8) & 0x00FF;
		data[1]=(gyro_x & 0x00FF);
		data[2]=(gyro_y >> 8) & 0x00FF;
		data[3]=(gyro_y & 0x00FF);
		data[4]=(gyro_z >> 8) & 0x00FF;
		data[5]=(gyro_z & 0x00FF);

		//can_clear_all_mob();
		//send can message to MOb
		while(can_cmd(&imu)!= CAN_CMD_ACCEPTED);
		//loop until send successful
		while(can_get_status(&imu) != CAN_STATUS_COMPLETED){
		}
		
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;
		data[3] = 0;
		data[4] = 0;
#endif
	}
}

