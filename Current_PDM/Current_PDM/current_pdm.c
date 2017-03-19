/*
 * Current_PDM.c
 *
 * Created: 1/4/2017 11:51:57 AM
 * Author : Jason McLoud
 * 
 * Current Sensing Power Distribution Module
 * Relay and Fuse Power Distribution Board with on-board current sensor
 * Current Sensors, hall effect, output 0-5v based on sensed Current
 * MCU reads values, stuffs all eight sensor readings into CAN packet and sends
 * MCU: AT90CAN128, CAN Transceiver: MCP2551, Current Sensor: ACS712 
 */ 


#include "current_pdm.h"

//Current FSAE vehicle specific definitions
#include "../../car_23.h"


int init(){
	/*
	Following two commands required to enable ADC channels 4-7 on AT90 series(JTAG shares pins)
	Setting the JTD bit requires two successive writes
	*/
	MCUCR |= (1 << JTD);
	MCUCR |= (1 << JTD);
	
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
	
	
	/*
	* Timer Init
	* Set Timer_1 to compare mode, with OCR
	* Prescalar = 64 with OCR set to 25000 for 10hz 
	* Data sent to can controller when compare match interrupt triggered
	*/
	Timer16_clear();
	Timer16_set_waveform_mode(TIMER16_WGM_CTC_OCR);
	Timer16_set_clock(TIMER16_CLKIO_BY_64);
	Timer16_set_compare_a(25000); //25000 ticks for compare match
	Timer16_compare_a_it_enable();

	return EXIT_SUCCESS;
}

int can_send(){
	
	 can_clear_all_mob();
	 //send can message to MOb
	 while(can_cmd(&msg)!= CAN_CMD_ACCEPTED);
	 //loop until send successful
	 while(can_get_status(&msg) != CAN_STATUS_COMPLETED);
	return EXIT_SUCCESS;
}



int main(void)
{
	/*
	* Initialize CanBus Message Objects
	*/
	msg.id.std = PDM;	
	msg.pt_data = &buffer[0];//point msg data to data buffer
	msg.cmd = CMD_TX;
	msg.dlc = 8;

	uint16_t  adc_result;
	init();
	
	/*
	* Read 8 ACS712 Hall Effect Current Sensors
	* Send through CANbus to Datalogger, CANID assigned in car23.h
	* Sensor ~66mV/A sensitivity
	* 10bit ADC, 4.88mV/LSB 
	*/
    while (TRUE) 
    {		int i =0;
			//Grab all eight sensor vals
			//TODO: channel 0 and 1 not working, only 2 through 7
			for(i = 0; i < 8; i++){
				adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, i);
				adc_result = adc_single_conversion(i);
				adc_result -= 512;					//acs712 offset, 0 = 2.5V
				
				//shift adc result to 8bit value, possible loss of one data point
				buffer[i] = (U8) ((adc_result >> 1) & 0x00ff);
				
			}			 
			
    }
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

