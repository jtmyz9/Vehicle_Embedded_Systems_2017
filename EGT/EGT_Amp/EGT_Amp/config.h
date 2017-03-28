/*
 * config.h
 *
 * Created: 2/28/2017 7:59:31 PM
 *  Author: MizzouRacing
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

//_____ I N C L U D E S ____________________________________________________

#include <avr/interrupt.h>
#include "compiler.h"
//#include <util/delay.h>




// -------------- MCU LIB CONFIGURATION
#define FOSC           8000        // 8Mhz internal clock
#define F_CPU          (FOSC*1000) // Need for AVR GCC

//!ADC PORT Defs
#define ADC_PORT_IN        PINF
#define ADC_PORT_DIR       DDRF
#define ADC_PORT_OUT      PORTF
#define ADC_DID_REG       DIDR0

//---------------- CAN DEFINITION -------------
#define     CAN_BAUDRATE        500



//------------------------------------------------------------------------------
//  @fn MACRO: Enable_interrupt
//!
//! Set the Global Interrupt flag (I) in SREG (Status REGister).
//!
//! @warning none
//!
//! @param none
//!
//! @return none
//!
//------------------------------------------------------------------------------
#define Enable_interrupt()      { asm ("sei"::) ; }

//------------------------------------------------------------------------------
//  @fn MACRO: Disable_interrupt
//!
//! Clear the Global Interrupt flag (I) in SREG (status register).
//!
//! @warning none
//!
//! @param none
//!
//! @return none
//!
//------------------------------------------------------------------------------
#define Disable_interrupt()     { asm ("cli"::) ; }
	
	//------------------------------------------------------------------------------
	//  @fn MACRO-FUNCTION Hard_reset
	//!
	//! RESET device with Watchdog Timer.
	//!
	//! @warning: Watchdog Timer used
	//!           Check BOOTRST fuse setting
	//!
	//! @param none
	//!
	//! @return none
	//!
	//------------------------------------------------------------------------------
#define Hard_reset()    { WDTCR |= 1<<WDE;  while(1); }


#endif /* CONFIG_H_ */