/*
 * Shift Light Test.cpp
 *
 * Created: 12/17/2016 10:00:52 PM
 * Author : Mizzou Racing
 */ 


#include <avr/io.h>
#include <string.h>

#include "config.h"
#include "can_lib.c"
#include "can_drv.c"

#include "shift_lights.c"
#include "uart.cpp"


#define RPM_ID 0x640
#define RPM_OFFSET 0
#define RPM_LENGTH 16

int main()
{	
	lights_init();
	lights_neutral();
//	uart_init();
//	while(1)
//		page_test();
//	can_init(0);
	
/*	U8 buffer[2];
	st_cmd_t rx_message;
	rx_message.id.std = 0x12;
	rx_message.cmd=CMD_RX_MASKED;
	rx_message.pt_data=buffer;
	
	while(1) {
		
		while(can_cmd(&rx_message) != CAN_CMD_ACCEPTED);
		while(can_get_status(&rx_message) == CAN_STATUS_NOT_COMPLETED);
			
			update_rpm((buffer[0] << 8) + buffer[1]);
			lights_rpm((buffer[0] << 8) + buffer[1]);
			_delay_ms(25);
	
		//lights_demo();
		//lights_overtemp();
	}
	//update_lights();
	//update_gear_position();
	//update_lcd();
	//watch for button interrupts
	//monitor adc
	//receive can
	//write serial lcd
	}
*/
	
ISR(TIMER1_OVF_vect){
	
}

