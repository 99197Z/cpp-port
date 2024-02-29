#include "vex.h"
#include "utills.h"

using namespace vex;

extern brain Brain;
extern smartdrive Drivetrain;
extern motor_group LeftDrive;
extern motor_group RightDrive;

void atton() {
	extendWings();
    LeftDrive.spin(forward,200,velocityUnits::rpm);
  	RightDrive.spin(forward,200,velocityUnits::rpm);
  	wait( 1300, timeUnits::msec );

	wait( 200, timeUnits::msec );

  	LeftDrive.spin(reverse,100,velocityUnits::rpm);
  	RightDrive.spin(reverse,100,velocityUnits::rpm);
  	wait( 500, timeUnits::msec );

  	LeftDrive.spin(reverse,200,velocityUnits::rpm);
  	RightDrive.spin(reverse,200,velocityUnits::rpm);
  	wait( 1000, timeUnits::msec );

	LeftDrive.spin(forward,200,velocityUnits::rpm);
  	RightDrive.spin(forward,200,velocityUnits::rpm);
  	wait( 700, timeUnits::msec );

	LeftDrive.spin(forward,200,velocityUnits::rpm);   // SPINNN
  	RightDrive.spin(forward,-200,velocityUnits::rpm); // SPINNN

  	Drivetrain.stop(brakeType::brake);
	retractWings();
}