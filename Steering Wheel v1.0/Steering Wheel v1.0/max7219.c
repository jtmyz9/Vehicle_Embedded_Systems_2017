/*
 * CFile1.c
 *
 * Created: 2/16/2017 3:46:39 PM
 *  Author: MizzouRacing
 */ 


/* MAX7219 Interaction Code
 * ---------------------------
 * For more information see
 * http://www.adnbr.co.uk/articles/max7219-and-7-segment-displays
 *
 * 668 bytes - ATmega168 - 16MHz
 */



#include <avr/io.h>
#include <util/delay.h>
#include "max7219.h"




void spiSendByte (char databyte)
{
    // Copy data into the SPI data register
    SPDR = databyte;
    // Wait until transfer is complete
    while (!(SPSR & (1 << SPIF)));
}

void MAX7219_writeData(char data_register, char data)
{
    MAX7219_LOAD0;
        // Send the register where the data will be stored
        spiSendByte(data_register);
        // Send the data to be stored
        spiSendByte(data);
    MAX7219_LOAD1;
}

void MAX7219_clearDisplay() 
{
        MAX7219_writeData(1, MAX7219_CHAR_BLANK);
}

void MAX7219_displayNumber(volatile long number) 
{
    MAX7219_clearDisplay();
	MAX7219_writeData(MAX7219_DIGIT0, number);
}

void MAX7219_display(int gear) {
		switch(gear) {
			case N:
				MAX7219_writeData(MAX7219_DIGIT0,0x15);
				break;
			case FIRST:
				MAX7219_writeData(MAX7219_DIGIT0,0x30);
				break;
			case SECOND:
				MAX7219_writeData(MAX7219_DIGIT0,0x6D);
				break;
			case THIRD:
				MAX7219_writeData(MAX7219_DIGIT0,0x79);
				break;
			case FOURTH:
				MAX7219_writeData(MAX7219_DIGIT0,0x33);
				break;
			case FIFTH:
				MAX7219_writeData(MAX7219_DIGIT0,0x5B);
				break;
			case SIXTH:
				MAX7219_writeData(MAX7219_DIGIT0,0x5F);
				break;
			case UNKNOWN:
				MAX7219_writeData(MAX7219_DIGIT0,0x01);
				break;
		}
}

void max_init()
{
    // SCK MOSI CS/LOAD/SS
    DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI) | (1 << PIN_SS);

    // SPI Enable, Master mode
    SPCR |= (1 << SPE) | (1 << MSTR)| (1<<SPR1);

    // Decode mode to no decode
    MAX7219_writeData(MAX7219_MODE_DECODE, 0x00);

    // Scan limit runs from 0.
    MAX7219_writeData(MAX7219_MODE_SCAN_LIMIT, 0);
    MAX7219_writeData(MAX7219_MODE_INTENSITY, 8);
    MAX7219_writeData(MAX7219_MODE_POWER, ON);

    
} 