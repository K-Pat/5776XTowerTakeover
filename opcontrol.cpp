#include "main.h"
#define LEFT_FRONT 2 //Sets a variable that holds the port value(allows for easy port changes)
#define RIGHT_FRONT 9
#define LEFT_BACK 3
#define RIGHT_BACK 8
#define LIFT 4
#define ANGLER 5
#define INTAKELEFT 6
#define INTAKERIGHT 7

pros::Motor Lf (LEFT_FRONT);//Lets the motor name and sets the port
pros::Motor Rf (RIGHT_FRONT, true); // This reverses the motor
pros::Motor Lb (LEFT_BACK);
pros::Motor Rb (RIGHT_BACK, true);
pros::Motor lift (LIFT);
pros::Motor angler (ANGLER);
pros::Motor intakeL (INTAKELEFT);
pros::Motor intakeR (INTAKERIGHT);
pros::Controller master (CONTROLLER_MASTER);

void Intake(){//Defines a function
	if(master.get_digital(DIGITAL_R1) == 1){//checks to see if certain button was pressed in an if statement
		intakeL.move(200);
		intakeR.move(-200);//moves at max speed
	}else if(master.get_digital(DIGITAL_R2) == 1){//else if
		intakeL.move(-200);
		intakeR.move(200);
	}else{//else
		intakeL.move_velocity(0);
		intakeR.move_velocity(0);
	}
}

void Angler(){
	if(master.get_digital(DIGITAL_L2) == 1){
		angler.move(100);
	}else if(master.get_digital(DIGITAL_L1) == 1){
		angler.move(-100);
	}else{
		angler.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);//sets a brake mode (in this case its hold)
		angler.move_velocity(0);//move_velocity has a built in PID and with the brake mode it stays in position even under a heavy load
	}
}

void tankDrive() {
  Lf.move(master.get_analog(ANALOG_LEFT_Y));
  Rf.move(master.get_analog(ANALOG_RIGHT_Y));//moves at a value that is gotten from the analog sticks
  Lb.move(master.get_analog(ANALOG_LEFT_Y));
  Rb.move(master.get_analog(ANALOG_RIGHT_Y));
}

void arcadeDrive() {
  int power = master.get_analog(ANALOG_LEFT_Y);
  int turn = master.get_analog(ANALOG_RIGHT_X);
  int left = power + turn;//Variable equals the vertical movement
  int right = power - turn;//variable equals the horizontal movement
  Lf.move(left);
  Lb.move(left);
  Rf.move(right);
  Rb.move(right);
}

void Lift(){
	if (master.get_digital(DIGITAL_UP) == 1){
		lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		lift.move_velocity(-200);
	}else if(master.get_digital(DIGITAL_DOWN)){
		lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		lift.move_velocity(200);
	}else{
		lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
		lift.move_velocity(0);
	}
}

void opcontrol() {//main function "Operational Control "
  while (true) {//An infinite while loop
    //tankDrive();
    arcadeDrive();//calling the functions
		Lift();
		Angler();
		Intake();
    pros::delay(10);//delay of ten milliseconds, prevents the code from crashing 
  }
}

