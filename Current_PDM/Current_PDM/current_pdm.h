/*
 * current_pdm.h
 *
 * Created: 1/14/2017 7:46:17 PM
 *  Author: MizzouRacing
 */ 


#ifndef CURRENT_PDM_H_
#define CURRENT_PDM_H_


#include <util/atomic.h>
#include <util/delay.h>
#include <avr/io.h>
#include "Includes/adc_drv.h"
#include "Includes/can_lib.h"
#include "Includes/timer16_drv.h"
#include "at90can_drv.h" 

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

U8 volatile buffer[8];
st_cmd_t msg;



#endif /* CURRENT_PDM_H_ */