/*
 * Steering Wheel v1.0.cpp
 *
 * Created: 3/16/2017 4:51:02 PM
 * Author : MizzouRacing
 */ 

#define F_CPU 16000000UL
#define RAND_MAX 9

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "shift_lights.c"
#include "max7219.c"
#include "can_lib.c"
#include "can_drv.c"

int main(void)
{
	// INITIALIZATION
	max_init();
	lights_init();
	led_strip_write(alloff, 20);
	//adc setup
	//ADMUX |= ((1 << REFS1 & 0 << REFS0))
	//can bus stuff
	Can_reset();						//reset can controller for initialization
	can_init(1);						//sets CANBT registers, and enables controller
	while ((CANGSTA & (1<<ENFG)) == 0); //! Wait for Enable OK
	CANGIT = 0xFF;                      //! Reset General errors and OVRTIM flag
	can_clear_all_mob();				//Clear all Mobs
	
	st_cmd_t rxmessage; //receive message stuff
	rxmessage.cmd = CMD_RX_MASKED;
	rxmessage.id.std = 0x631;
	U8 rxbuffer[2];
	rxmessage.pt_data = &rxbuffer[0];
	rxmessage.dlc = 2;
	rxmessage.ctrl.ide = 0;
	rxmessage.ctrl.rtr = 0;
	
	st_cmd_t txmessage; //transmit message stuff
	txmessage.cmd = CMD_TX;
	txmessage.id.std = 0x632;
	U8 txbuffer[8];
	txmessage.pt_data = &txbuffer[0];
	txmessage.dlc = 8;
	txmessage.ctrl.ide = 0;
	txmessage.ctrl.rtr = 0;
	
	
	
	
	 
	 int gear_position = UNKNOWN;
	 MAX7219_display(gear_position);

	



	while(1) {
	int i;
	for(i=0;i<2;i++) rxbuffer[i]=0;
		
	while(can_cmd(&rxmessage) != CAN_CMD_ACCEPTED);
		
	while(can_get_status(&rxmessage) != CAN_STATUS_COMPLETED);
		
	int pressure = (rxbuffer[1] << 8) | rxbuffer[0];
	if(pressure < 200) lights_rpm(0);
	else if(pressure < 300) lights_rpm(1);
	else if(pressure < 400) lights_rpm(2);
	else if(pressure < 500) lights_rpm(3);
	else if(pressure < 600) lights_rpm(4);
	else if(pressure < 700) lights_rpm(5);
	else if(pressure < 800) lights_rpm(6);
	else if(pressure < 900) lights_rpm(7);
	}
}

