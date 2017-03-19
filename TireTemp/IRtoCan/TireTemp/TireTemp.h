/*
 * TireTemp.h
 *
 * Created: 1/14/2017 12:07:02 PM
 *  Author: MizzouRacing
 */ 


#ifndef TIRETEMP_H_
#define TIRETEMP_H_


#include <util/atomic.h>
#include <avr/io.h>
#include "compiler.h"
#include "config.h"
#include "can/can_lib.h"

#include "timer/timer16_drv.h"
#include "at90can_drv.h"
#include "car_23.h"

//data holders
U16 tyre_out, tyre_mid, tyre_in, brake;
U8 buffer[8];
st_cmd_t msg;




#endif /* TIRETEMP_H_ */