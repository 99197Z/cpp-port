/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Ben Hunt                                                  */
/*    Created:      2/7/2024, 3:46:19 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "utills.h"
#include "atton.h"
#include <iostream>


using namespace vex;
using std::cout;
using std::endl;

// A global instance of competition
competition Competition;
brain Brain;

led LedR1 = led(Brain.ThreeWirePort.A);
led LedR2 = led(Brain.ThreeWirePort.B);
led LedY1 = led(Brain.ThreeWirePort.C);
led LedY2 = led(Brain.ThreeWirePort.D);

inertial Inertial = inertial(PORT1);

motor MotorLf = motor(PORT11, ratio18_1, true);
motor MotorLb = motor(PORT12, ratio18_1, true);
motor_group LeftDrive = motor_group(MotorLf, MotorLb);

motor MotorRf = motor(PORT19, ratio18_1, false); 
motor MotorRb = motor(PORT20, ratio18_1, false); 
motor_group RightDrive = motor_group(MotorRf, MotorRb);
//drivetrain Drivetrain = drivetrain(LeftDrive, RightDrive, 319.19, 320, 165, mm, 1);

motor MotorPuncher = motor(PORT15, ratio18_1, true); 

motor MotorIntakeL = motor(PORT14, ratio18_1, true);
motor MotorIntakeR = motor(PORT16, ratio18_1, true);
motor_group IntakeGroup = motor_group(MotorIntakeL, MotorIntakeR);


motor MotorWingL = motor(PORT13, ratio18_1, true);
motor MotorWingR = motor(PORT17, ratio18_1, false);
motor_group WingGroup = motor_group(MotorWingL, MotorWingR);

smartdrive Drivetrain= smartdrive(LeftDrive, RightDrive, Inertial, 319.19, 320, 165, mm, 1);// 7 / 5
int L = 0;
int R = 0;
semaphore semaphore_leds;

controller Controller1 = controller(primary);

struct buttons
{
	bool up;
	bool down;
} Buttons;


// define your global instances of motors and other devices here




/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  LedR1.set(0);
  LedR2.set(0);
  LedY1.set(0);
  LedY2.set(0);
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  Inertial.startCalibration();
  wait(2000,timeUnits::msec);
  std::cout << "Inertial Calibration" << std::endl;
  Controller1.rumble(".");
  
}


/*---------------------------------------------------------------------------*/
/*                             Threaded Functions                            */
/*---------------------------------------------------------------------------*/
int displayTask() {
    while(1) {
      	// display some useful info
      	Brain.Screen.setCursor(2,1);
      	Brain.Screen.print( "  MotorLb    speed: %4.0f   position: %6.2f", MotorLb.velocity( percent ), MotorLb.position( rev ) );
      	Brain.Screen.newLine();
      	Brain.Screen.print( "  MotorLf    speed: %4.0f   position: %6.2f", MotorLf.velocity( percent ), MotorLf.position( rev ));
      	Brain.Screen.newLine();
      	Brain.Screen.print( "  MotorRb    speed: %4.0f   position: %6.2f", MotorRb.velocity( percent ), MotorRb.position( rev ));
      	Brain.Screen.newLine();
      	Brain.Screen.print( "  MotorRf    speed: %4.0f   position: %6.2f", MotorRf.velocity( percent ), MotorRf.position( rev ));
      	Brain.Screen.newLine();
      	Brain.Screen.newLine();

      	// motor group velocity and position is returned for the first motor in the group
      	Brain.Screen.print( "  leftDrive  speed: %4.0f   position: %6.2f", LeftDrive.velocity( percent ), LeftDrive.position( rev ));
      	Brain.Screen.newLine();
      	Brain.Screen.print( "  rightDrive speed: %4.0f   position: %6.2f", RightDrive.velocity( percent ), RightDrive.position( rev ));
      	Brain.Screen.newLine();
      	Brain.Screen.newLine();

      	// drivetrain velocity is the average of the motor velocities for left and right
      	Brain.Screen.print( "  Drivetrain speed: %4.0f", Drivetrain.velocity( percent ) );
      	Brain.Screen.newLine();

      	Controller1.Screen.setCursor(1,1);
      	Controller1.Screen.print("Temp %.1f",ConvertPCTdegC(Drivetrain.temperature(percent)));

      	// no need to run this loop too quickly
      	wait( 20, timeUnits::msec );
    }

    return 0;
}

int logTask() {
	if (Brain.SDcard.isInserted()) {
		uint8_t tempBuf[0];
    	Brain.SDcard.savefile("match.bin", tempBuf, 0);
		while (1)
		{
    	  	union thing {
    	  	    uint8_t result[20];  // 4 per int  // 12 for 3
    	  	    struct loggedData {
    	  	      int Lf_temp;
    	  	      int Lb_temp;
    	  	      int Rf_temp;
				  int Rb_temp;
				  int Puncher_temp;
    	  	    } motors;
    	  	} t;
    	  	t.motors.Lf_temp      = MotorLf.temperature(temperatureUnits::celsius);
			t.motors.Lb_temp      = MotorLb.temperature(temperatureUnits::celsius);
			t.motors.Rf_temp      = MotorRf.temperature(temperatureUnits::celsius);
			t.motors.Rb_temp      = MotorRb.temperature(temperatureUnits::celsius);
			t.motors.Puncher_temp = MotorPuncher.temperature(temperatureUnits::celsius);
			Brain.SDcard.appendfile("match.bin", t.result, sizeof(t.result));
			wait( 500, timeUnits::msec );
		}
		return 0;
	} else {
		display(0b1001);
		wait( 2000, timeUnits::msec );
		display(0b0000);
		return -1;
	}
	
}

/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  	display(0b0011);
  	// ..........................................................................
  	// Insert autonomous user code here.
  	// ..........................................................................
  	LeftDrive.spin(forward,200,velocityUnits::rpm);
  	RightDrive.spin(forward,200,velocityUnits::rpm);
  	wait( 1300, timeUnits::msec );

	MotorPuncher.spin(forward,200,velocityUnits::rpm);
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

/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  	// User control code here, inside the loop
	MotorPuncher.stop(brakeType::coast);
  	Drivetrain.stop(brakeType::brake);
  	while (1) {
    	// This is the main execution loop for the user control program.
    	// Each time through the loop your program should update motor + servo
    	// values based on feedback from the joysticks.

    	// ........................................................................
    	// Insert user code here. This is where you use the joystick values to
    	// update your motors, etc.
    	// ........................................................................
		L = Controller1.Axis3.position();
    	R = Controller1.Axis2.position();
    	LeftDrive.spin(forward,L,percent);
    	RightDrive.spin(forward,R,percent);

		// Puncher
		if (Controller1.ButtonR2.pressing()) {
			MotorPuncher.spin(forward,200,velocityUnits::rpm);
		} else {
			MotorPuncher.spin(forward,0,velocityUnits::rpm);
		}

		// intake
		if (Controller1.ButtonL2.pressing()) {
			IntakeGroup.spin(forward,200,velocityUnits::rpm);
		} else if (Controller1.ButtonL1.pressing()) {
			IntakeGroup.spin(reverse,200,velocityUnits::rpm);
		} else {
			IntakeGroup.spin(forward,0,velocityUnits::rpm);
		}


		//wings
		if (Controller1.ButtonUp.pressing()) {
			if (!Buttons.up) {
				Buttons.up = true;
				toggleWings();
			}
		} else {
			Buttons.up = false;
		}

    	wait(20, msec); // Sleep the task for a short amount of time to
    	                // prevent wasted resources.
  	}
}

// Main will set up the competition functions and callbacks.
int main() {
  // Set up callbacks for autonomous and driver control periods.
  task displayTaskInstance( displayTask );
  task task(logTask);
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  	// Run the pre-autonomous function.
  	pre_auton();

  	// Prevent main from exiting with an infinite loop.
  	while (true) {
  	  	wait(100, msec);
  	}
}
