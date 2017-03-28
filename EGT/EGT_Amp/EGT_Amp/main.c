/*
 * EGT_Amp.c
 *
 * Created: 3/7/2017 1:01:10 AM
 * Author : MizzouRacing
 */ 

#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "can_lib.h"
//#define FOSC 8000 // In KHz

//#ifdef DEBUG
	//
	//#define LIN_BAUDRATE 9600 // In bit/s
	//#define CONF_LINBRR 25
//
	//#define Lin_set_baudrate(br) \
	//{ LINBRRH = (unsigned char)(((unsigned short)br)>>8);\
	//LINBRRL = (unsigned char) ((unsigned short)br); }
//
	//#define Lin_set_data(data) ( LINDAT = data )
//
	//void uart_put_string (unsigned char *data_string)
	//{
		//while(*data_string++){ 
			//Lin_set_data(*data_string);
			////data_string++;
			//};
	//}
//#endif


	
int main(void)
{
	//DDRD = (1<<DDD3);
	
	/*
	* CAN Bus Enable section
	* MUST SET CAN_BAUDRATE in config.h 
	*/	
	//Can_reset();						//reset can controller for initialization
	can_init(1);						//sets CANBT registers, and enables controller
//	while ((CANGSTA & (1<<ENFG)) == 0); //! Wait for Enable OK
//	CANGIT = 0xFF;                      //! Reset General errors and OVRTIM flag
//	can_clear_all_mob();				//Clear all Mobs
	////end CAN Bus init

//#ifdef DEBUG	
	//LINCR = (1<<LENA) | (1<<LCMD2) | (1<<LCMD1) | (1<<LCMD0) | (0<<LCONF1) | (0<<LCONF0);
	//LINCR = 0b00001111;
	//LINBRR = 25;
	//Lin_set_baudrate(CONF_LINBRR);
	//unsigned char *i = "test\n";
//#endif	
	
	U8  buffer[8];
	st_cmd_t msg;
	
	//init message as tx
	msg.id.std = 0x123;
	msg.pt_data = &buffer[0];//point msg data to data buffer
	msg.cmd = CMD_TX;
	msg.dlc = 2;
	buffer[0] = 0xA0;
	
   
	U16 result =-1;
    while (1)
    {
//
	//adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, 5);
	//result = adc_single_conversion(5);
			//uart_put_string(i);
				//
		//send can message to MOb
		while(can_cmd(&msg) != CAN_CMD_ACCEPTED);
		while(can_get_status(&msg) == CAN_STATUS_NOT_COMPLETED){
		
		}
		//buffer[0]++;
		//_delay_ms(100);
    }
}

/*
Interrrupt Service routine for timer compare match 
	based on setting in init_systimer

At each interrupt handles sending Thermocouple Measurements 
	to Can Controller Mob
*/
//ISR(TCOMP_1){
//
//}

