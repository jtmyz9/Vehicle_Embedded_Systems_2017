/*
 * uart.cpp
 *
 * Created: 1/14/2017 11:24:10 PM
 *  Author: Max Houck
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

#define F_CPU 16000000UL
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void uart_init() {
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); //enable transmit and receive
	UCSR0C = (0<<UMSEL0) | (0<<UPM00) | (0<<UPM01) | (0<<USBS0) | (1<<UCSZ00) | (1<<UCSZ01); //n,8,1
	
	UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRR0L = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
}

void page_test() {
			while((UCSR0A & (1 << UDRE0)) == 0) {};
			UDR0 = 0x08;
			while((UCSR0A & (1 << UDRE0)) == 0) {};
			UDR0 = 0x03;
			
			TCCR1B |= (1<<CS12 | 1 << CS10);//timer setup
			TCNT1=0;
			
			while(1) {
				int rpm,test;
				for(rpm = 0;rpm<13500;rpm+=100) {
					while((UCSR0A & (1 << UDRE0)) == 0) {};
					UDR0 = 0x00;
					while((UCSR0A & (1 << UDRE0)) == 0) {};
					UDR0 = (rpm & 0xFF);
					while((UCSR0A & (1 << UDRE0)) == 0) {};
					UDR0 = (rpm >> 8);
					
					while(TCNT1 < 781);
					TCNT1=0;
					}
				while((UCSR0A & (1 << UDRE0)) == 0) {};
				UDR0 = 0x0C;
				while((UCSR0A & (1 << UDRE0)) == 0) {};
				UDR0= 0x6C;
			}
}

void update_rpm(uint16_t rpm) {
	while((UCSR0A & (1 << UDRE0)) == 0) {};
	UDR0 = 0x00;
	while((UCSR0A & (1 << UDRE0)) == 0) {};
	UDR0 = (uint8_t) rpm; //send lower byte
	while((UCSR0A & (1 << UDRE0)) == 0) {};
	UDR0 = (uint8_t) (rpm >> 8); //send higher byte
//	_delay_ms(50);
}