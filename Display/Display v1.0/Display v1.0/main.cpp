/*
 * Display v1.0.cpp
 *
 * Created: 2/8/2017 8:12:55 PM
 * Author : Mizzou Racing
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "max7219.c"

int main(void)
{
	
	max_init();
	
	    MAX7219_LOAD0;
	    // Send the register where the data will be stored
	    spiSendByte(0x01);
	    // Send the data to be stored
	    spiSendByte(0x08);
	    MAX7219_LOAD1;
/*
	int i;
	while(1)
	{
		for(i=0;i<10;i++)
		MAX7219_displayNumber(--i);
		_delay_ms(10);
	}
	*/
	
	
	uart_init();
	
	/* Replace with your application code */
	//   while (1)
	{
		page_test();
	}
}
