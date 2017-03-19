/*
 * TireTemp
 *
 * Created: 8/12/2015 6:57:52 PM
 *  Author: Jason McLoud
 */ 

#include "TireTemp.h"
#include "i2c/mlx90614.h"
#include "i2c/i2cmaster.h"

/*
* Setup Timer/counter for 10ms overflow
* Send data out on canbus at 100hz
* 10hz might be sufficient
	*/
int init_systimer() {
	Timer16_clear();
	Timer16_set_waveform_mode(TIMER16_WGM_CTC_OCR);
	Timer16_set_clock(TIMER16_CLKIO_BY_64);
	Timer16_set_compare_a(2500); //2500 ticks for compare match
	Timer16_compare_a_it_enable();
}


void can_send(){
	
	//store temp, in buffer
	buffer[0] = (tyre_out<<8)>>8 & 0x00FF;
	buffer[1] = tyre_out>>8 & 0x00FF;;
	buffer[2] = (tyre_mid<<8)>>8 & 0x00FF;;
	buffer[3] = tyre_mid>>8 & 0x00FF;;
	buffer[4] = (tyre_in<<8)>>8 & 0x00FF;;
	buffer[5] = tyre_in>>8 & 0x00FF;;
	buffer[6] = (brake<<8)>>8 & 0x00FF;;
	buffer[7] = brake>>8 & 0x00FF;;
	
	can_clear_all_mob();
	//send can message to MOb
	while(can_cmd(&msg)!= CAN_CMD_ACCEPTED);
	//loop until send successful
	while(can_get_status(&msg) == CAN_STATUS_NOT_COMPLETED);
}

int main(void)
{		
	//i2c initialization macro
	i2c_init();	
	
	
	
	/*
	* CAN Bus Enable section
	* MUST SET CAN_BAUDRATE in config.h 
	*/	
	Can_reset();						//reset can controller for initialization
	can_init(1);						//sets CANBT registers, and enables controller
	while ((CANGSTA & (1<<ENFG)) == 0); //! Wait for Enable OK
	CANGIT = 0xFF;                      //! Reset General errors and OVRTIM flag
	can_clear_all_mob();				//Clear all Mobs
	////end CAN Bus init              //! Reset General errors and OVRTIM flag
	
	
	
	
	msg.id.std = LEFT_FRONT;
	//msg.handle = can_get_mob_free();
	//.......
	msg.pt_data = &buffer[0];//point msg data to data buffer	
	msg.cmd = CMD_TX;
	msg.dlc = 8;
	
	init_systimer();
	
	while(1){
		
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			//retrieve temperature from i2c temp sensor(s)
			tyre_out = (int)mlx_getObjTempF(OUTER);
			tyre_mid = (int)mlx_getObjTempF(MIDDLE);
			tyre_in = (int)mlx_getObjTempF(INNER);
			brake = (int)mlx_getObjTempF(BRAKE);
		}	
	
		
		
	}
	
return 0;
}

/*
* Timer0 compare match Interrupt
* Interrupt Used to time data output 
* Compare value and prescalar set in init to determine timer sequence
*/
ISR(TIMER1_COMPA_vect){
	
	Disable_interrupt();
	Timer16_compare_a_it_disable();
	Timer16_set_compare_a(2500); // Reset Output compare to 10ms
	
	can_send();
	Timer16_compare_a_it_enable();
	Enable_interrupt();
}