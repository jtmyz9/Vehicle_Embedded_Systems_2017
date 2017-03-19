#include "ws2812_config.h"
#include "light_ws2812.h"

 //defined as g,r,b
const struct cRGB green = {0,0,0};
const struct cRGB yellow = {0,0,0};
const struct cRGB orange = {0,0,0};
const struct cRGB red = {0,0,0};
const struct cRGB off = {0,0,0};
const uint16_t numleds = 20;

struct cRGB *ledarray;

const struct cRGB *fulltree = {green,green,yellow,yellow,orange,orange,red,red,red,red
		red,red,red,red,orange,orange,yellow,yellow,green,green};
const struct cRGB *alloff


//accepts a number 0 (no lights) to 7 (shift now) and sets led's accordingly
void lights_set_rpm(int stage);


//flashing red lights for overheat
void lights_set_overtemp();

//all green for neutral and idle
void lights_set_neutral();

//all orange for engine off and but in gear
void lights_set_ingear();

