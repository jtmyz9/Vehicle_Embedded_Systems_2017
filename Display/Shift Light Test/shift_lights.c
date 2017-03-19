#include "shift_lights.h"

//accepts a number 0 (no lights) to 7 (shift now) and sets led's accordingly
void lights_set_rpm(int stage){
	switch(stage) {
		case 0:
			
	}
	ws2812_setleds(ledarray,numleds)
}

//flashing red lights for overheat
void lights_set_overtemp() {
	
}

//all green for neutral and idle
void lights_set_neutral() {
	
}

//all orange for engine off and but in gear
void lights_set_ingear() {
	
}
