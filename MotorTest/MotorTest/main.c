/*
 * MotorTest.c
 *
 * Created: 1/17/2017 5:23:41 PM
 * Author : MizzouRacing
 */ 

#include <avr/io.h>
#include "timer16_drv.h"
#include "config.h"

#ifndef USE_TIMER16
#define  USE_TIMER16 TIMER16_1
#endif

int main(void)
{
	DDRA = (1<<DDA0) | (1<<DDA1);
	PORTA = (1<<PA0); 
	
	DDRB |= (1<<DDB7);										//set bit 7 of port b as output

	
	Timer16_set_counter(0);// clear counter
	Timer16_set_mode_output_c(TIMER16_COMP_MODE_CLEAR_OC); //clear OCR on upcount, standard duty cycle
	Timer16_set_waveform_mode(TIMER16_WGM_PWM_PFC_ICR); // phase and freq correct, ICR == top
	Timer16_set_clock(TIMER16_CLKIO_BY_1);				//default clock, prescaler 0
	Timer16_set_capture(36363); //220 Hz from 16 MHz clock
	
    /* Replace with your application code */
	
	Timer16_set_pwm_c(16000);	//change duty cycle of TCC1C
    while (1) 
    {
		int i = 7;
		while(1)i++;
    }
}

