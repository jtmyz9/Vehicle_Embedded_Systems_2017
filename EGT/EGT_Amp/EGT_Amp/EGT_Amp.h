/*
 * EGT_Amp.h
 *
 * Created: 3/28/2017 10:34:40 AM
 *  Author: MizzouRacing
 */ 


#ifndef EGT_AMP_H_
#define EGT_AMP_H_

#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "can_lib.h"
#include "adc_drv.h"
#include "systimer.h"
#include <util/atomic.h>
#include <util/delay.h>

static void sys_init(void);
static void io_init(void);
void handle_10ms_tick(void);



#ifdef DEBUG

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
#endif

//! Thermocouple OpAmp ADC Channel
#define OPAMP_1 5
#define OPAMP_2 6
#define OPAMP_3 7
#define OPAMP_4 8
#define OPAMP_5 9

//! Thermocouple conversion factors
#define Celsius			5		//5 mv/C
#define THERMO_SCALE	1000	//scale ADC to mv


st_cmd_t egt;
U8 volatile thermo_val[8];
U32 ADC_result;


#endif /* EGT_AMP_H_ */