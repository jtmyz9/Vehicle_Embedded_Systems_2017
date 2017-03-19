/*
 * EGT.h
 *
 * Created: 3/1/2017 11:18:58 AM
 *  Author: MizzouRacing
 */ 


#ifndef EGT_H_
#define EGT_H_

//_____ I N C L U D E S ____________________________________________________

#include <avr/io.h>
#include "config.h"


//_____ D E C L A R A T I O N S ________________________________________________

typedef unsigned char      Bool;

typedef unsigned char       U8 ;
typedef unsigned short      U16;
typedef unsigned long       U32;
typedef unsigned long long  U64;
typedef signed char         S8 ;
typedef signed short        S16;
typedef signed long         S32;
typedef signed long long    S64;


//______ADC PORT/PIN DEFS____________

#define  CHANNEL_1		PIND4
#define  CHANNEL_2		PIND5
#define  CHANNEL_3		PIND6
#define  CHANNEL_4		PINB7
#define  CHANNEL_5		PINB2
#define  CHANNEL_6		PINB5
#define  CHANNEL_7		PINB6
#define  CHANNEL_8		PINC4
#define  CHANNEL_9		PINC5
#define  CHANNEL_10		PINC6

#define  CHANNEL_1_DDR		DDRD
#define  CHANNEL_2_DDR		DDRD
#define  CHANNEL_3_DDR		DDRD
#define  CHANNEL_4_DDR		DDRB
#define  CHANNEL_5_DDR		DDRB
#define  CHANNEL_6_DDR		DDRB
#define  CHANNEL_7_DDR		DDRB
#define  CHANNEL_8_DDR		DDRC
#define  CHANNEL_9_DDR		DDRC
#define  CHANNEL_10_DDR		DDRC


#endif /* EGT_H_ */