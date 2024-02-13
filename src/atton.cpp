#include "vex.h"
#include "utills.h"

using namespace vex;

extern brain Brain;
extern smartdrive Drivetrain;
extern motor_group LeftDrive;
extern motor_group RightDrive;
extern motor MotorPuncher;

void atton() {
    LeftDrive.spin(forward,200,velocityUnits::rpm);
  	RightDrive.spin(forward,200,velocityUnits::rpm);
  	wait( 1300, timeUnits::msec );

	MotorPuncher.spin(forward,200,velocityUnits::rpm); // Puncher
	wait( 200, timeUnits::msec );

  	LeftDrive.spin(reverse,100,velocityUnits::rpm);
  	RightDrive.spin(reverse,100,velocityUnits::rpm);
  	wait( 500, timeUnits::msec );

  	LeftDrive.spin(reverse,200,velocityUnits::rpm);
  	RightDrive.spin(reverse,200,velocityUnits::rpm);
  	wait( 1000, timeUnits::msec );

	MotorPuncher.stop(brakeType::coast);
  	Drivetrain.stop(brakeType::brake);
}