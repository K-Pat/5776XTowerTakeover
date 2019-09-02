#include "main.h"
#define LEFT_FRONT 2
#define RIGHT_FRONT 9
#define LEFT_BACK 3
#define RIGHT_BACK 8
#define LIFT 4

pros::Motor Lf (LEFT_FRONT);
pros::Motor Rf (RIGHT_FRONT, true); // This reverses the motor
pros::Motor Lb (LEFT_BACK);
pros::Motor Rb (RIGHT_BACK, true);
pros::Motor lift (LIFT);
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

void Macro1(){
	if (master.get_digital(DIGITAL_X) == 1){
		lift.tare_position();
		double position = lift.get_encoder_units();
		double target = 1000;
		double Kp = 0.2;
		double speed = (target - position)*Kp;
		while(position<target){
			lift.move(speed);
		}
	}
}

void Macro2(){
	if (master.get_digital(DIGITAL_X) == 1){
		lift.tare_position();
		double position = lift.get_encoder_units();
		double target = 1000;
		double Kp = 0.2;
		double speed = (target - position)*Kp;
		while(position<target){
			lift.move(speed);
		}
	}
}

void opcontrol() {
  while (true) {
    //tankDrive();
    arcadeDrive();
    pros::delay(10);
  }
}
