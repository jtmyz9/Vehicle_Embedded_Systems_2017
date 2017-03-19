/*
 * max7219.h
 *
 * Created: 2/16/2017 3:55:32 PM
 *  Author: MizzouRacing
 */ 


#ifndef MAX7219_H_
#define MAX7219_H_


// 16MHz clock
#define F_CPU 16000000UL

// Outputs, pin definitions
#define PIN_SCK                   PORTB1
#define PIN_MOSI                  PORTB2
#define PIN_SS                    PORTB0

#define ON                        1
#define OFF                       0

#define MAX7219_LOAD1             PORTB |= (1<<PIN_SS)
#define MAX7219_LOAD0             PORTB &= ~(1<<PIN_SS)

#define MAX7219_MODE_DECODE       0x09
#define MAX7219_MODE_INTENSITY    0x0A
#define MAX7219_MODE_SCAN_LIMIT   0x0B
#define MAX7219_MODE_POWER        0x0C
#define MAX7219_MODE_TEST         0x0F
#define MAX7219_MODE_NOOP         0x00

// I only have 3 digits, no point having the
// rest. You could use more though.
#define MAX7219_DIGIT0            0x01
#define MAX7219_DIGIT1            0x02
#define MAX7219_DIGIT2            0x03

#define MAX7219_CHAR_BLANK        0xF
#define MAX7219_CHAR_NEGATIVE     0xA

enum Gear {N, FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, UNKNOWN};

void spiSendByte (char databyte);
void MAX7219_writeData(char data_register, char data);
void MAX7219_clearDisplay();
//void MAX7219_displayNumber(volatile long number);
void MAX7219_display(int gear);
void max_init();

#endif /* MAX7219_H_ */