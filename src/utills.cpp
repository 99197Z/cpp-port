#include "vex.h"

using namespace vex;

extern motor_group IntakeGroup;
int intakeSpeed = 100;

extern led LedR1;
extern led LedR2;
extern led LedY1;
extern led LedY2;
extern semaphore semaphore_leds;

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

// Intake
void IntakeIn() {
    IntakeGroup.spin(forward,intakeSpeed,rpm);
}
void IntakeOut() {
    IntakeGroup.spin(reverse,intakeSpeed,rpm);
}

void IntakeStop() {
    IntakeGroup.spin(forward,0,rpm);
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