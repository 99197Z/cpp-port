#include "vex.h"

#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ui.h"

using namespace vex;

using std::cout;
using std::endl;
using std::string;

extern motor_group WingGroup;

enum wingsState : unsigned int { retracted, extended };
wingsState WingsPos = wingsState::retracted;
wingsState WingsGoal = wingsState::retracted;

double driveGearRatio = 7/5;

extern led LedR1;
extern led LedR2;
extern led LedY1;
extern led LedY2;
extern smartdrive Drivetrain;
extern semaphore semaphore_leds;

extern motor MotorLf;
extern motor MotorLb;
extern motor MotorRf;
extern motor MotorRb;

extern controller Controller1;

#include <memory>
#include <string>
using std::string;
using std::max;

int wingMoveAtempts = 0;


double ConvertPCTdegC(double percent) {
    //(percent)                           = (deg - 21.0) / (100 - 21.0) * 100;
    //(percent/100)                       = (deg - 21.0) / (100 - 21.0)
    //(percent/100) * (100 - 21.0)        = (deg - 21.0)
    //(percent/100) * (100 - 21.0) + 21.0 = deg

    return ((percent/100)*(100 - 21.0))+21.0;
}

double max4(double a, double b, double c, double d) {
    return max(max(a, b), max(c, d));
}

double motorTemps() {
    return max4(
        MotorLf.temperature(temperatureUnits::celsius),
        MotorLb.temperature(temperatureUnits::celsius),
        MotorRf.temperature(temperatureUnits::celsius),
        MotorRb.temperature(temperatureUnits::celsius)
    );
}


/*---------------------------------------------------------------------------*/
/*                             External  Hardware                            */
/*---------------------------------------------------------------------------*/

// Wings
void extendWings() {
    if (WingsPos == wingsState::retracted) {
        WingGroup.spinToPosition(135,rotationUnits::deg,true);
        WingsPos = wingsState::extended;
        //WingGroup.setVelocity(1,rpm);
        //WingGroup.spin(forward);
        WingGroup.stop(brakeType::brake);
    }
}

void retractWings() {
    if (WingsPos == wingsState::extended) {
        WingGroup.spinToPosition(0,rotationUnits::deg,true);
        WingsPos = wingsState::retracted;
    }
}

void toggleWings() {
    if (WingsGoal == wingsState::retracted) {
        WingsGoal = wingsState::extended;
    } else if (WingsGoal == wingsState::extended) {
        WingsGoal = wingsState::retracted;
    }
}

int hardwareTask() {
    std::cout << "Hardware Started" << std::endl;
    while (1) {
        if (WingsPos != WingsGoal) {
            wingMoveAtempts++;
            std::cout << "Wings" << std::endl;
            if (wingMoveAtempts >= 5) {
                std::cout << "Wings move failed" << std::endl;
                Controller1.rumble("...");
                return -22;
            }
            if (WingsPos == wingsState::retracted) {
                extendWings();
                wingMoveAtempts--;
            } else if (WingsPos == wingsState::extended) {
                retractWings();
                wingMoveAtempts--;
            }

        }

        wait(125, msec);
    }
    
}

double wheelRPM(double motorRPM) {
    return driveGearRatio*motorRPM;
}
double DriveSpeed(double motorRPM) {
    return (2*M_PI*(50.8))/(wheelRPM(motorRPM)/60)/1000;
}
double DriveDistance(double motorRPM,double time) {
    return (DriveSpeed(motorRPM)*time);
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