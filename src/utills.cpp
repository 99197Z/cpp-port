#include "vex.h"

using namespace vex;

extern motor_group WingGroup;

enum wingsState : unsigned int { retracted, extended };
wingsState WingsPos = wingsState::retracted;


extern led LedR1;
extern led LedR2;
extern led LedY1;
extern led LedY2;
extern semaphore semaphore_leds;
#include <memory>
#include <string>
using std::string;


double ConvertPCTdegC(double percent) {
    //(percent)                           = (deg - 21.0) / (100 - 21.0) * 100;
    //(percent/100)                       = (deg - 21.0) / (100 - 21.0)
    //(percent/100) * (100 - 21.0)        = (deg - 21.0)
    //(percent/100) * (100 - 21.0) + 21.0 = deg

    return ((percent/100)*(100 - 21.0))+21.0;
}


/*---------------------------------------------------------------------------*/
/*                             External  Hardware                            */
/*---------------------------------------------------------------------------*/

// Wings
void extendWings() {
    if (WingsPos == wingsState::retracted) {
        WingGroup.spinToPosition(90,rotationUnits::deg,true);
        WingsPos = wingsState::extended;
    }
}

void retractWings() {
    if (WingsPos == wingsState::extended) {
        WingGroup.spinToPosition(0,rotationUnits::deg,true);
        WingsPos = wingsState::retracted;
    }
}

void toggleWings() {
    if (WingsPos == wingsState::retracted) {
        extendWings();
        WingsPos = wingsState::extended;
    } else if (WingsPos == wingsState::extended) {
        retractWings();
        WingsPos = wingsState::retracted;
    }
}


// Led
void display(int code) {
	semaphore_leds.lock();
    LedR1.set(code&8);
    LedR2.set(code&4);
    LedY1.set(code&2);
    LedY2.set(code&1);
	wait( 1500, timeUnits::msec );
	semaphore_leds.unlock();
}