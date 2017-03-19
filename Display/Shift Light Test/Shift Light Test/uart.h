/*
 * uart.h
 *
 * Created: 1/14/2017 11:24:31 PM
 *  Author: Max Houck
 */ 

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void uart_init();