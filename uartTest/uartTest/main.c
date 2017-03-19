/*
 * uartTest.c
 *
 * Created: 1/6/2017 5:44:16 PM
 * Author : MizzouRacing
 */ 

#include <avr/io.h>
#include "uart_lib.h"
#include "uart_drv.h"


int main(void)
{
	uart_init(CONF_8BIT_NOPAR_1STOP, 9600);
    /* Replace with your application code */
    while (1) 
    {
		uart_put_string("test");
    }
}

