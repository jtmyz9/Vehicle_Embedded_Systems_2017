/*
 * EGT_Amp.c
 * Board Includes 5 Thermocouple OpAmps
 * Program cycles through 5 ADC channels attached to OpAmps
 * Data is stored in global structure, and sent and determined rate to canbus
 *
 *
 * Created: 3/7/2017 1:01:10 AM
 * Author : Jason McLoud
 */ 

#include "EGT_Amp.h"

/*
* EGT Main loop
* Check thermocouple reading every 5ms and save to global can msg
*/
int main(void)
{	
	Disable_interrupt();
	sys_init();
	
	ADC_result = -1;
    while (1)
    {

		adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, OPAMP_1);
		ADC_result = adc_single_conversion(OPAMP_1);
		//convert ADC result to mv
		ADC_result *= 5000;
		ADC_result /= 1023;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
				thermo_val[0] = ADC_result / Celsius;
				
			}
		_delay_ms(1);
		
		adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, OPAMP_2);
		ADC_result = adc_single_conversion(OPAMP_2);
		//convert ADC result to mv
		ADC_result *= 5000;
		ADC_result /= 1023;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			thermo_val[1] = ADC_result/ Celsius ;
		
		}
		_delay_ms(1);
		
		adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, OPAMP_3);
		ADC_result = adc_single_conversion(OPAMP_3);
		//convert ADC result to mv
		ADC_result *= 5000;
		ADC_result /= 1023;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			thermo_val[2] = ADC_result / Celsius;
		
		}
		_delay_ms(1);
		
		adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, OPAMP_4);
		ADC_result = adc_single_conversion(OPAMP_4);
		//convert ADC result to mv
		ADC_result *= 5000;
		ADC_result /= 1023;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			thermo_val[3] = ADC_result / Celsius;
		
		}
		_delay_ms(1);
		
		adc_init(AVCC_AS_VREF, NO_LEFT_ADJUST, OPAMP_5);
		ADC_result = adc_single_conversion(OPAMP_5);
		//convert ADC result to mv
		ADC_result = 5000*ADC_result/1023;
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			thermo_val[4] = (U8) ADC_result / Celsius;
			
		}
	
		
		_delay_ms(5);
    }
}


void sys_init(void) {
	
#ifdef DEBUG
	LINCR = (1<<LENA) | (1<<LCMD2) | (1<<LCMD1) | (1<<LCMD0) | (0<<LCONF1) | (0<<LCONF0);
	LINCR = 0b00001111;
	LINBRR = 25;
	Lin_set_baudrate(CONF_LINBRR);
	unsigned char *i = "test\n";
#endif

	// Make sure sys clock is at least 8MHz
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	systimer_10ms = 0;
	
	//init message as tx
	egt.id.std = EGT_AMP;
	egt.pt_data = &thermo_val[0];//point msg data pointer to data buffer
	egt.cmd = CMD_TX;
	egt.dlc = 8;
#ifdef DEBUG
	int i =0;
	for(i; i< 8; i++){
		thermo_val[i] = 0xA0;
	}	
	
#endif
	io_init();		//init i/o 
	can_init(1);	//init can bus
	
	//init timer/counter overflow to 1250 for 100hz output compare match
	systimer_10ms_restart(5);
	Enable_interrupt();
}

void io_init(void) {
	
	// Init PORTB[7:0]
	DDRB = 0xFF;	
	PORTB = 0xFF;
	
	// Init PORTC[7:0] // PORTC[3:2] => RXCAN:TXCAN
	DDRC = 0x00;
	PORTC = 0x00;
	
	DDRC |= (1<<DDC1);	//LED Pin Enable
	PORTC |= (1<<PINC1);
	
	// Init PORTD[7:0]
	DDRD = 0x00;
	PORTD = 0x00;
	
	// Init PORTE[2:0]
	DDRE = 0x00;
	PORTD = 0x00;
}


/*
*	Called by timer compare match ISR
*	Compare Vector set to trigger on 10ms intervals
*	handle_10ms_tick: handle for sending messages to can controller
*	USAGE: set data output frequency, based upon current systimer val
*/
void handle_10ms_tick(void)
{
	if(systimer_10ms %10 == 0)
	{
		
	}

	if(systimer_10ms %20 == 0)
	{
		// Send 50Hz Data
		
	}

	if(systimer_10ms % 50 == 0)
	{
		// Send 20Hz Data
		
	}

	if(systimer_10ms % 100 == 0)
	{
		// Send 10Hz Data
		

	}

	if(systimer_10ms % 200 == 0)
	{
		// Send 5Hz Data
		
	}
	if(systimer_10ms % 500 == 0)
	{
		// Send 2Hz Data
		
	}
	if(systimer_10ms % 1000 == 0)
	{	
		//Send 1Hz Data
		//send can message to MOb
		while(can_cmd(&egt) != CAN_CMD_ACCEPTED);
		while(can_get_status(&egt) == CAN_STATUS_NOT_COMPLETED);
		PORTC ^= (1<<1);
	}
}
