/*
 * config.h
 *
 * Created: 2/28/2017 7:59:31 PM
 *  Author: MizzouRacing
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

//_____ I N C L U D E S ____________________________________________________
#include <avr/io.h>
#include <avr/interrupt.h>
#include "compiler.h"

// -------------- MCU LIB CONFIGURATION
#define FOSC           8000        // 8 MHz internal RC osc
#define F_CPU          (FOSC*1000) // Needed for AVR GCC

//!ADC PORT Defs
#define ADC_PORT_IN        PINF
#define ADC_PORT_DIR       DDRF
#define ADC_PORT_OUT      PORTF
#define ADC_DID_REG       DIDR0

//---------------- CAN DEFINITION -------------
#define     CAN_BAUDRATE        500

typedef unsigned char      Bool;

typedef unsigned char       U8 ;
typedef unsigned short      U16;
typedef unsigned long       U32;
typedef unsigned long long  U64;
typedef signed char         S8 ;
typedef signed short        S16;
typedef signed long         S32;
typedef signed long long    S64;

typedef union
{
	U16 h   ;     // h as HALF-WORD
	U8  b[2];     // b as BYTE
} Union16;

typedef union
{
	U32 w   ;     // w as WORD
	U16 h[2];     // h as HALF-WORD
	U8  b[4];     // b as BYTE
} Union32;

typedef union
{
	U64 d   ;     // d as DOUBLE-WORD
	U32 w[2];     // w as WORD
	U16 h[4];     // h as HALF-WORD
	U8  b[8];     // b as BYTE
} Union64;

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