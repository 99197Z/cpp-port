#ifndef UTILLS_H
#define UTILLS_H

enum wingsState: unsigned int;

double ConvertPCTdegC(double percent);
void display(int code);

void extendWings();
void retractWings();
void toggleWings();
int hardwareTask();

double motorTemps();

double DriveDistance(double motorRPM,double time);

#endif