#ifndef UTILLS_H
#define UTILLS_H

enum wingsState: unsigned int;

double ConvertPCTdegC(double percent);
void display(int code);

enum wingsState: unsigned int;
void extendWings();
void retractWings();
void toggleWings();

void IntakeIn();
void IntakeOut();
void IntakeStop();


#endif