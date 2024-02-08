/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       38302008                                                  */
/*    Created:      2/7/2024, 3:46:19 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <iostream>


using namespace vex;
using std::cout;
using std::endl;

// A global instance of compet
competition Competition;
brain  Brain;

led LedR1 = led(Brain.ThreeWirePort.A);
led LedR2 = led(Brain.ThreeWirePort.B);
led LedY1 = led(Brain.ThreeWirePort.C);
led LedY2 = led(Brain.ThreeWirePort.D);

motor MotorLf = motor(PORT1, ratio18_1, false);
motor MotorLb = motor(PORT10, ratio18_1, false);
motor_group LeftDrive = motor_group(MotorLf, MotorLb);

motor MotorRf = motor(PORT11, ratio18_1, true); 
motor MotorRb = motor(PORT20, ratio18_1, true); 
motor_group RightDrive = motor_group(MotorRf, MotorRb);
drivetrain Drivetrain = drivetrain(LeftDrive, RightDrive, 319.19, 320, 165, mm, 1);
int L = 0;
int R = 0;


controller Controller1 = controller(primary);

// define your global instances of motors and other devices here


void display(int code) {
    LedR1.set(code&8);
    LedR2.set(code&4);
    LedY1.set(code&2);
    LedY2.set(code&1);
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  Controller1.rumble(".");
  std::cout << "E" << std::endl;
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}


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
      Brain.Screen.print( "  robotDrive speed: %4.0f", Drivetrain.velocity( percent ) );
      Brain.Screen.newLine();

      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print(" AVG Temp %.1f",Drivetrain.temperature(percent));

      // no need to run this loop too quickly
      wait( 20, timeUnits::msec );
    }

    return 0;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  display(0b0011);
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  LeftDrive.spin(forward,200,velocityUnits::rpm);
  RightDrive.spin(forward,200,velocityUnits::rpm);
  wait( 1500, timeUnits::msec );
  LeftDrive.spin(reverse,100,velocityUnits::rpm);
  RightDrive.spin(reverse,100,velocityUnits::rpm);
  wait( 500, timeUnits::msec );
  LeftDrive.spin(reverse,200,velocityUnits::rpm);
  RightDrive.spin(reverse,200,velocityUnits::rpm);
  wait( 1000, timeUnits::msec );
  Drivetrain.stop(brakeType::brake);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    L = Controller1.Axis3.position();
    R = Controller1.Axis2.position();
    LeftDrive.spin(forward,L,percent);
    RightDrive.spin(forward,R,percent);
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  task displayTaskInstance( displayTask );
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
