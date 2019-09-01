#include "main.h"
#define LEFT_FRONT 2
#define RIGHT_FRONT 9
#define LEFT_BACK 3
#define RIGHT_BACK 8

pros::Motor Lf (LEFT_FRONT);
pros::Motor Rf (RIGHT_FRONT, true); // This reverses the motor
pros::Motor Lb (LEFT_BACK);
pros::Motor Rb (RIGHT_BACK, true);
pros::Controller master (CONTROLLER_MASTER);

void tankDrive() {
  Lf.move(master.get_analog(ANALOG_LEFT_Y));
  Rf.move(master.get_analog(ANALOG_RIGHT_Y));
  Lb.move(master.get_analog(ANALOG_LEFT_Y));
  Rb.move(master.get_analog(ANALOG_RIGHT_Y));
}

void arcadeDrive() {
  int power = master.get_analog(ANALOG_LEFT_Y);
  int turn = master.get_analog(ANALOG_RIGHT_X);
  int left = power + turn;
  int right = power - turn;
  Lf.move(left);
  Lb.move(left);
  Rf.move(right);
  Rb.move(right);
}

void opcontrol() {
  while (true) {
    //tankDrive();
    arcadeDrive();
    pros::delay(10);
  }
}
