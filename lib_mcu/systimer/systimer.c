/*
 * systimer.c
 *
 *  Created on: Feb 11, 2012
 *      Author: Administrator
 */
#include "systimer.h"

uint32_t systimer_10ms = 0;

void systimer_10ms_restart(uint8_t offset)
{
	//Clear counter
	TCNT1 = 0;
	// Set for slot delay (using OCRnB)
	if(offset != 0)
	{
		OCR1A = 250 * offset; //1ms*offset
	}
	else
	{
		OCR1A = 2500;
	}
	TIMSK1 = (1<<OCIE1A);
	TCCR1B = 0x0B; // CTC mode and clk/64

	if(offset == 0)
	{
		systimer_10ms += 10;
		handle_10ms_tick();

	}
}

ISR(TIMER1_COMPA_vect)
{
	TIMSK1 &= (0<<OCIE1A);
	OCR1A = 2500; // Reset Output compare to 10ms
	systimer_10ms += 10;
	handle_10ms_tick(); // Call the tick handling function
	TIMSK1 |= (1<<OCIE1A);
}
