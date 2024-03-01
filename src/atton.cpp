#include "vex.h"
#include "utills.h"
#include "ui.h"
#include <string>

using namespace vex;

extern brain Brain;
extern smartdrive Drivetrain;
extern motor_group LeftDrive;
extern motor_group RightDrive;

extern int autonToRun;

void atton() {
	if (autonToRun == 4) {
		LeftDrive.spin(forward,150,velocityUnits::rpm);
  		RightDrive.spin(forward,150,velocityUnits::rpm);
  		wait( 1500, timeUnits::msec );


  		LeftDrive.spin(reverse,150,velocityUnits::rpm);
  		RightDrive.spin(reverse,150,velocityUnits::rpm);
  		wait( 1500, timeUnits::msec );
	}
    LeftDrive.spin(forward,150,velocityUnits::rpm);
  	RightDrive.spin(forward,150,velocityUnits::rpm);
  	wait( 1000, timeUnits::msec );


  	LeftDrive.spin(reverse,150,velocityUnits::rpm);
  	RightDrive.spin(reverse,150,velocityUnits::rpm);
  	wait( 1000, timeUnits::msec );

  	Drivetrain.stop(brakeType::brake);
}