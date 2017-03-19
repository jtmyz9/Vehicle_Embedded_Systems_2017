/*
 * EGT_Amp.c
 *
 * Created: 3/7/2017 1:01:10 AM
 * Author : MizzouRacing
 */ 

#include <avr/io.h>
//

#define FOSC 8000 // In KHz
#define LIN_BAUDRATE 9600 // In bit/s
#define CONF_LINBRR 25

#define Lin_set_baudrate(br) \
{ LINBRRH = (unsigned char)(((unsigned short)br)>>8);\
LINBRRL = (unsigned char) ((unsigned short)br); }

#define Lin_set_data(data) ( LINDAT = data )

void uart_put_string (unsigned char *data_string)
{
	while(*data_string++){ 
		Lin_set_data(*data_string);
		//data_string++;
		};
}

int main(void)
{
	DDRD = (1<<DDD3);
	
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
	
//	LINCR = (1<<LENA) | (1<<LCMD2) | (1<<LCMD1) | (1<<LCMD0) | (0<<LCONF1) | (0<<LCONF0);
//	LINCR = 0b00001111;
	//LINBRR = 25;
//	Lin_set_baudrate(CONF_LINBRR);
//	unsigned char *i = "test\n";
	
    /* Replace with your application code */
//	U16 result =-1;
    while (1)
    {
//		adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, 5);
//		result = adc_single_conversion(5);
			//uart_put_string(i);
				
		
		
    }
}

