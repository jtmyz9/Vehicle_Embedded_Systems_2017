// These lines specify what pin the LED strip is on.
#define LED_STRIP_PORT PORTB
#define LED_STRIP_DDR  DDRB
#define LED_STRIP_PIN  4

//defines at what rpm the stage begins
#define stage0 0
#define stage1 7000
#define stage2 8000
#define stage3 9000
#define stage4 10000
#define stage5 11000
#define stage6 12000
#define stage7 13000
//The rgb_color struct represents the color for an 8-bit RGB LED.
typedef struct color
{
	unsigned char red, green, blue;
} color;

#define LED_COUNT 20

 static color green = {0,150,0};
 static color yellow = {75,50,0};
 static color red = {200,0,0};
 static color orange = {100,30,0};
 static color off = {0,0,0};
 static color white = {255,255,255};
	 
 color fulltree[20];
 color allred[20];
 color alloff[20];
 color allorange[20];
 color allgreen[20];
 color allwhite[20];
 
 enum blink {BLINK_DISABLED,BLINK_ON,BLINK_OFF} blink;
 int timercounter;	 	

enum lights {NEUTRAL,ONE,TWO,THREE,FOUR,FIVE,SIX,SHIFT,INGEAR,COOLANT,FLASHLIGHT};
//call this in a loop in main.c
void lights_update();

void lights_flashlight();
void lights_overtemp();
void lights_neutral();
void lights_ingear();
void lights_init();
void lights_rpm(int);


/** led_strip_write sends a series of colors to the LED strip, updating the LEDs.
 The colors parameter should point to an array of color structs that hold the colors to send.
 The count parameter is the number of colors to send.
 This function takes about 1.1 ms to update 30 LEDs.
 Interrupts must be disabled during that time, so any interrupt-based library
 can be negatively affected by this function.
 Timing details sat 20 MHz (the numbers slightly different at 16 MHz and 8MHz):
  0 pulse  = 400 ns
  1 pulse  = 850 ns
  "period" = 1300 ns
 */
void __attribute__((noinline)) led_strip_write(color* , unsigned int );

void lights_demo();