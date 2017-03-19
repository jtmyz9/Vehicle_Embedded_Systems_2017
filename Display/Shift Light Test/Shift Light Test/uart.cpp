/*
 * uart.cpp
 *
 * Created: 1/14/2017 11:24:10 PM
 *  Author: Max Houck
 */ 
#include "uart.h"

void uart_init() {
	UCSR0B = (1 << RXEN) | (1 << TXEN); //enable transmit and receive
	UCSR0C = (0<<UMSEL0) | (0<<UPM0) | (0<<USBS0) | (3<<UCSZ0); //n,8,1
	
	UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRR0L = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
}

void page_test() {
	while((UCSR0A & (1 << UDRE)) == 0) {};
	UDR0 = 0x08;
	while((UCSR0A & (1 << UDRE)) == 0) {};
	UDR0 = 0x01;
	_delay_ms(50);
}

void update_rpm(uint16_t rpm) {
	while((UCSR0A & (1 << UDRE)) == 0) {};
	UDR0 = 0x00;
	while((UCSR0A & (1 << UDRE)) == 0) {};
	UDR0 = (uint8_t) rpm; //send lower byte
	while((UCSR0A & (1 << UDRE)) == 0) {};
	UDR0 = (uint8_t) (rpm >> 8); //send higher byte
	_delay_ms(50);
}