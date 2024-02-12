#include "utills.h"
#include "vex.h"

using namespace vex;

extern led LedR1;
extern led LedR2;
extern led LedY1;
extern led LedY2;
extern semaphore semaphore_leds;

double ConvertPCTdegC(double percent) {
    //return (percent - 21.0) / (100 - 21.0) * 100;
    return ((percent/100)*(100 - 21.0))+21.0;
}

void display(int code) {
	semaphore_leds.lock();
    LedR1.set(code&8);
    LedR2.set(code&4);
    LedY1.set(code&2);
    LedY2.set(code&1);
	wait( 1500, timeUnits::msec );
	semaphore_leds.unlock();
}