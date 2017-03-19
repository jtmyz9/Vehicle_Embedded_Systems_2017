/* Author: Max Houck
	Created 12/18/2016
		Modified from Polulo library
 */

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "shift_lights.h"

void lights_init() {
	fulltree[0] = green;
	fulltree[1] = green;
	fulltree[2] = yellow;
	fulltree[3] = yellow;
	fulltree[4] = orange;
	fulltree[5] = orange;
	fulltree[6] = red;
	fulltree[7] = red;
	fulltree[8] = red;
	fulltree[9] = red;
	fulltree[10] = red;
	fulltree[11] = red;
	fulltree[12] = red;
	fulltree[13] = red;
	fulltree[14] = orange;
	fulltree[15] = orange;
	fulltree[16] = yellow;
	fulltree[17] = yellow;
	fulltree[18] = green;
	fulltree[19] = green;
	
	for(int i = 0; i<20; i++)
		allred[i] = red;	
	for(int i = 0; i<20; i++)
		allgreen[i] = green;	
	for(int i = 0; i<20; i++)
		alloff[i] = off;	
	for(int i = 0; i<20; i++)
		allwhite[i] = white;
	for(int i = 0; i<20; i++)
		allorange[i] = orange;
		
	blink = BLINK_DISABLED;
}

void lights_rpm(int rpm) {
	color partialtree[20];
	memcpy(partialtree,fulltree,sizeof(color)*20);
	int stage;
	if(rpm < stage1) stage = 0;
	else if (rpm < stage2) stage = 1;
	else if (rpm < stage3) stage = 2;
	else if (rpm < stage4) stage = 3;
	else if (rpm < stage5) stage = 4;
	else if (rpm < stage6) stage = 5;
	else if (rpm <  stage7) stage = 6;
	else stage = 7;
	switch(stage) {
		case 7: led_strip_write(fulltree,LED_COUNT);
				break;
		case 0: partialtree[0] = off;
				partialtree[19] = off;
		case 1: partialtree[1] = off;
				partialtree[18] = off;
		case 2: partialtree[2] = off;
				partialtree[17] = off;
		case 3: partialtree[3] = off;
				partialtree[16] = off;
		case 4: partialtree[4] = off;
				partialtree[15] = off;
		case 5: partialtree[5] = off;
				partialtree[14] = off;
		case 6: for(int i = 6; i <= 13; i++)
					partialtree[i] = off;
				led_strip_write(partialtree,LED_COUNT);
	}
}

void lights_flashlight() {
	led_strip_write(allwhite,LED_COUNT);
}

void lights_neutral() {
	led_strip_write(allgreen,LED_COUNT);
}

void lights_ingear(){
	led_strip_write(allorange,LED_COUNT);
}

void lights_overtemp(){
	if (blink == BLINK_DISABLED) {
		TCCR1B |= (1<<CS12 | 1 << CS10); //set clock select to fcpu / 1024
		TCNT1 = 0;
		led_strip_write(allred,20);
		blink = BLINK_ON;
	}
	else if(TCNT1 >= 7812) {
		TCNT1 = 0;
		if(blink == BLINK_ON) {
			led_strip_write(alloff,20);
			blink = BLINK_OFF;
			TCNT1=0;
		}
		else if(blink == BLINK_OFF) {
			led_strip_write(allred,20);
			blink = BLINK_ON;
			TCNT1=0;
		}
	}
}

void lights_demo() {
			for(int i = 0; i < 8; i++) {
				lights_rpm(i);
				_delay_ms(500);
			}
			lights_ingear();
			_delay_ms(2500);
			lights_neutral();
			_delay_ms(2500);
			lights_flashlight();
			_delay_ms(2500);
}

void lights_update() {
	lights_init();
	//while(1)
		//if neutral and idle
	//if ingear and idle
	//if overtemp
	//if flashlight
	//else update rpm
}

void __attribute__((noinline)) led_strip_write(color *colors, unsigned int count) 
{
  // Set the pin to be an output driving low.
  LED_STRIP_PORT &= ~(1<<LED_STRIP_PIN);
  LED_STRIP_DDR |= (1<<LED_STRIP_PIN);

  cli();   // Disable interrupts temporarily because we don't want our pulse timing to be messed up.
  while(count--)
  {
    // Send a color to the LED strip.
    // The assembly below also increments the 'colors' pointer,
    // it will be pointing to the next color at the end of this loop.
    asm volatile(
        "ld __tmp_reg__, %a0+\n"
        "ld __tmp_reg__, %a0\n"
        "rcall send_led_strip_byte%=\n"  // Send red component.
        "ld __tmp_reg__, -%a0\n"
        "rcall send_led_strip_byte%=\n"  // Send green component.
        "ld __tmp_reg__, %a0+\n"
        "ld __tmp_reg__, %a0+\n"
        "ld __tmp_reg__, %a0+\n"
        "rcall send_led_strip_byte%=\n"  // Send blue component.
        "rjmp led_strip_asm_end%=\n"     // Jump past the assembly subroutines.

        // send_led_strip_byte subroutine:  Sends a byte to the LED strip.
        "send_led_strip_byte%=:\n"
        "rcall send_led_strip_bit%=\n"  // Send most-significant bit (bit 7).
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"
        "rcall send_led_strip_bit%=\n"  // Send least-significant bit (bit 0).
        "ret\n"

        // send_led_strip_bit subroutine:  Sends single bit to the LED strip by driving the data line
        // high for some time.  The amount of time the line is high depends on whether the bit is 0 or 1,
        // but this function always takes the same time (2 us).
        "send_led_strip_bit%=:\n"
#if F_CPU == 8000000
        "rol __tmp_reg__\n"                      // Rotate left through carry.
#endif
        "sbi %2, %3\n"                           // Drive the line high.

#if F_CPU != 8000000
        "rol __tmp_reg__\n"                      // Rotate left through carry.
#endif

#if F_CPU == 16000000
        "nop\n" "nop\n"
#elif F_CPU == 20000000
        "nop\n" "nop\n" "nop\n" "nop\n"
#elif F_CPU != 8000000
#error "Unsupported F_CPU"
#endif

        "brcs .+2\n" "cbi %2, %3\n"              // If the bit to send is 0, drive the line low now.

#if F_CPU == 8000000
        "nop\n" "nop\n"
#elif F_CPU == 16000000
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
#elif F_CPU == 20000000
        "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
        "nop\n" "nop\n"
#endif

        "brcc .+2\n" "cbi %2, %3\n"              // If the bit to send is 1, drive the line low now.

        "ret\n"
        "led_strip_asm_end%=: "
        : "=b" (colors)
        : "0" (colors),         // %a0 points to the next color to display
          "I" (_SFR_IO_ADDR(LED_STRIP_PORT)),   // %2 is the port register (e.g. PORTC)
          "I" (LED_STRIP_PIN)     // %3 is the pin number (0-8)
    );

    // Uncomment the line below to temporarily enable interrupts between each color.
   // sei(); asm volatile("nop\n"); cli();
  }
  sei();          // Re-enable interrupts now that we are done.
  _delay_us(80);  // Send the reset signal.
}