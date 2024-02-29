#include "vex.h"
#include "utills.h"

using namespace vex;

extern brain Brain;
extern smartdrive Drivetrain;
extern motor_group LeftDrive;
extern motor_group RightDrive;
extern motor MotorPuncher;

void atton() {
    LeftDrive.spin(forward,150,velocityUnits::rpm);
  	RightDrive.spin(forward,150,velocityUnits::rpm);
  	wait( 1000, timeUnits::msec );


  	LeftDrive.spin(reverse,150,velocityUnits::rpm);
  	RightDrive.spin(reverse,150,velocityUnits::rpm);
  	wait( 1000, timeUnits::msec );

	MotorPuncher.stop(brakeType::coast);
  	Drivetrain.stop(brakeType::brake);
}