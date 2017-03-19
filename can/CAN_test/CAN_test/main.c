/*
 * CAN_test.c
 *
 *
 * Quick test of CAN Bus send and receive
 *		Test Proper operation of AT90CAN and MCP2551
 * Send and Receive message in loop
 * Use Arduino with CanBus shield for testing
 * Send Different Message to ensure that reception is working 
 */ 

#include <avr/io.h>
#include "config.h"
#include "can_lib.h"


int main(void)
{
	
	/*
	* CAN Bus Enable section
	* MUST SET CAN_BAUDRATE in config.h 
	*/	
	Can_reset();						//reset can controller for initialization
	can_init(1);						//sets CANBT registers, and enables controller
	while ((CANGSTA & (1<<ENFG)) == 0); //! Wait for Enable OK
	CANGIT = 0xFF;                      //! Reset General errors and OVRTIM flag
	can_clear_all_mob();				//Clear all Mobs
	////end CAN Bus init
	
	
	U8 volatile buffer[8];
	st_cmd_t msg;
	
	//init message as tx
	msg.id.std = 0x123;
	msg.pt_data = &buffer[0];//point msg data to data buffer
	msg.cmd = CMD_TX;
	msg.dlc = 8;
	
	
    /* Replace with your application code */
    while (1) 
    {	
		
		//send can message to MOb
		while(can_cmd(&msg)!= CAN_CMD_ACCEPTED);
		//loop until send successful
		while(can_get_status(&msg) != CAN_STATUS_COMPLETED){
			if(can_get_status(&msg) ==  CAN_STATUS_ERROR){
				can_clear_all_mob();
				break;
			}
		}
		buffer[0]++;
		
		
		msg.cmd = CMD_RX_MASKED;
		msg.id.std = 0x122;
		
		while(can_cmd(&msg) != CAN_CMD_ACCEPTED);		//uses id value stored in struct for id, and sets acceptance mask to all 1
		while(can_get_status(&msg) != CAN_STATUS_COMPLETED);
		
    }
}

