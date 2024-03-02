#include "vex.h"
#include "utills.h"
#include <string>
#include <iostream>
using std::cout;
using std::endl;

using namespace vex;

extern brain Brain;
extern competition Competition;

extern smartdrive Drivetrain;
extern motor MotorLf;
extern motor MotorLb;
extern motor MotorRf;
extern motor MotorRb;
extern controller Controller1;
extern motor_group LeftDrive;
extern motor_group RightDrive;


extern int autonToRun;

class Button {
  public:
    int x, y, width, height;
    std::string text;
    vex::color buttonColor, textColor;

	Button(int x, int y, int width, int height, std::string text, vex::color buttonColor, vex::color textColor)
		: x(x), y(y), width(width), height(height), text(text), buttonColor(buttonColor), textColor(textColor) {}

	void render()
    {
      Brain.Screen.drawRectangle(x, y, width, height, buttonColor);
      Brain.Screen.printAt(x + 10, y + 10, false, text.c_str());
    }

    bool isClicked()
    {
      if(Brain.Screen.pressing() && Brain.Screen.xPosition() >= x && Brain.Screen.xPosition() <= x + width &&
      Brain.Screen.yPosition() >= y && Brain.Screen.yPosition() <= y + width) return true;
      return false;
    }
};

Button autonButtons[] = {
  Button(10, 10, 150, 50, "Auton Red 1", vex::green, vex::white),
  Button(170, 10, 150, 50, "Auton Red 2", vex::black, vex::white),
  Button(10, 70, 150, 50, "Auton Blue 1", vex::black, vex::white),
  Button(170, 70, 150, 50, "Auton Blue 2", vex::black, vex::white),
  Button(170, 150, 150, 50, "Auton Skills", vex::orange, vex::white)
};


bool autonSel = true;

int UITask() {
    std::cout << "Display Started" << std::endl;
    while (autonSel)
    {
        Brain.Screen.clearScreen(vex::black);

        if(!Competition.isEnabled())
        {
          for(int i = 0; i < 5; i++)
          {
            autonButtons[i].render();
            if(autonButtons[i].isClicked())
            {
              autonButtons[autonToRun].buttonColor = vex::white;
              autonButtons[i].buttonColor = vex::green;
              autonToRun = i;
              autonSel = false;
            }
          }
        } else {
            autonSel = false;
        }

        Brain.Screen.render();
        wait( 20, timeUnits::msec );
    }
    std::cout << "Main Display Started" << std::endl;
    Brain.Screen.clearScreen(vex::black);
    while(1) {
        Brain.Screen.clearScreen(vex::black);
      	// display some useful info
      	Brain.Screen.setCursor(2,1);
      	Brain.Screen.print( "  MotorLb    speed: %4.0f   temps *C: %6.2f", MotorLb.velocity( percent ), MotorLb.temperature(temperatureUnits::celsius) );
      	Brain.Screen.newLine();
      	Brain.Screen.print( "  MotorLf    speed: %4.0f   temps *C: %6.2f", MotorLf.velocity( percent ), MotorLf.temperature(temperatureUnits::celsius));
      	Brain.Screen.newLine();
      	Brain.Screen.print( "  MotorRb    speed: %4.0f   temps *C: %6.2f", MotorRb.velocity( percent ), MotorRb.temperature(temperatureUnits::celsius));
      	Brain.Screen.newLine();
      	Brain.Screen.print( "  MotorRf    speed: %4.0f   temps *C: %6.2f", MotorRf.velocity( percent ), MotorRf.temperature(temperatureUnits::celsius));
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
        Controller1.Screen.print("D %2.0f*C|B %2.0f*C %1.0fV",motorTemps(),Brain.Battery.temperature(celsius),Brain.Battery.voltage());

        // no need to run this loop too quickly
        Brain.Screen.render();
        wait( 20, timeUnits::msec );
    }

    return 0;
    
}