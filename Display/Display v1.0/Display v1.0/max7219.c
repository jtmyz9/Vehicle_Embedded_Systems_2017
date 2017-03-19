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
	
	/*
    char negative = 0;

    // Convert negative to positive.
    // Keep a record that it was negative so we can
    // sign it again on the display.
    if (number < 0) {
        negative = 1;
        number *= -1;
    }*/

    MAX7219_clearDisplay();
	MAX7219_writeData(MAX7219_DIGIT0, number);
    // If number = 0, only show one zero then exit
   /* if (number == 0) {
        MAX7219_writeData(MAX7219_DIGIT0, 0);
        return;
    }*/
    
    // Initialization to 0 required in this case,
    // does not work without it. Not sure why.
  //  char i = 0; 
    
    // Loop until number is 0.
  /*  do {
        MAX7219_writeData(++i, number % 10);
        // Actually divide by 10 now.
        number /= 10;
    } while (number);*/

    // Bear in mind that if you only have three digits, and
    // try to display something like "-256" all that will display
    // will be "256" because it needs an extra fourth digit to
    // display the sign.

}


void max_init()
{
    // SCK MOSI CS/LOAD/SS
    DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI) | (1 << PIN_SS);

    // SPI Enable, Master mode
    SPCR |= (1 << SPE) | (1 << MSTR)| (1<<SPR1);

    // Decode mode to "Font Code-B"
    MAX7219_writeData(MAX7219_MODE_DECODE, 0xFF);

    // Scan limit runs from 0.
    MAX7219_writeData(MAX7219_MODE_SCAN_LIMIT, 0);
    MAX7219_writeData(MAX7219_MODE_INTENSITY, 8);
    MAX7219_writeData(MAX7219_MODE_POWER, ON);

    
} 