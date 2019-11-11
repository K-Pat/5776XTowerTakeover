#include "main.h"

#define LEFT_FRONT 12 //Sets a variable that holds the port value(allows for easy port changes)
#define RIGHT_FRONT 19
#define LEFT_BACK 13
#define RIGHT_BACK 18
#define LIFT 14
#define ANGLER 15
#define INTAKELEFT 16
#define INTAKERIGHT 17

#include "okapi/api.hpp"
using namespace okapi;
using namespace okapi::literals;

pros::Motor Lf (LEFT_FRONT);//Lets the motor name and sets the port
pros::Motor Rf (RIGHT_FRONT, true); // This reverses the motor
pros::Motor Lb (LEFT_BACK);
pros::Motor Rb (RIGHT_BACK, true);
pros::Motor lift (LIFT);
pros::Motor angler (ANGLER);
pros::Motor intakeL (INTAKELEFT);
pros::Motor intakeR (INTAKERIGHT, true);
pros::Controller master (CONTROLLER_MASTER);

okapi::MotorGroup intakeMotor({16, -17});

auto drive = okapi::ChassisControllerFactory::create(//creates an abstract motor or "Chassis Controller"
		 {12, 13}, //left motor ports
     {-19, -18},//right motor ports (- represents reversing ports)
		 okapi::AbstractMotor::gearset::green,//type of gearset
		 {3.25_in, 12.5_in}//3.25 inch wheels, 12.5 inch wheel base
 );

 auto profileController = AsyncControllerFactory::motionProfile(
   1,  // Maximum linear velocity of the Chassis in m/s
   2,  // Maximum linear acceleration of the Chassis in m/s/s
   5, // Maximum linear jerk of the Chassis in m/s/s/s
   drive // Chassis Controller
 );



void move(Point target){//move function that uses okapi's 2d motion profile
 	profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, target},  "a");
 	profileController.setTarget("a");
 	profileController.waitUntilSettled();

 }

 void moveb(Point target){//move function using 2 motion profile to go backward(blocks while robot is driving)
 	profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, target}, "a");
 	profileController.setTarget("a", true);//true inverts drive
 	profileController.waitUntilSettled();
 }

 void FrontBlue(){}

 void FrontRed(){}

 void BackBlue(){
   intakeMotor.moveVelocity(200);
   move(Point{4_ft, 0_ft, 0_deg});
   drive.turnAngle(35_deg);
	 move(Point{1_ft, 0_ft, 0_deg});
   drive.turnAngle(90_deg);
   move(Point{3.8_ft, 0_ft, 0_deg});
  intakeMotor.moveVelocity(0);
   angler.move_velocity(-80);
   pros::delay(2000);
   moveb(Point{1_ft,0_ft,0_deg});
 }

 void BackRed(){}

 void Intake(){//Defines a function
 	if(master.get_digital(DIGITAL_R1) == 1){//checks to see if certain button was pressed in an if statement
 		intakeL.move(200);
 		intakeR.move(200);//moves at max speep:O) d
 	}else if(master.get_digital(DIGITAL_R2) == 1){//else if
 		intakeL.move(-55);
 		intakeR.move(-55);
 	}else{//else
 		intakeL.move_velocity(0);
 		intakeR.move_velocity(0);
 	}
 }

 void Angler(){
 	if(master.get_digital(DIGITAL_L2) == 1){
 		angler.move(80);
 	}else if(master.get_digital(DIGITAL_L1) == 1){
 		angler.move(-80);
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

 void Macro(){
 	lift.tare_position();
 	double Kp = 0.2;//subject to testing (constant for Proportional)
	double Ki = 0.2;//Subject to testing (constant for Integral)
	double Kd = 0.2;//Subject to testing (constant for Derivative)
 	double target = 3600;//how far we need to go
	double integral_max_value = 100;//Testing required
	while(lift.get_position() < target){// Keeps the PID Going until target is reached
 	double position = lift.get_position();
 	double error = target - position;//how far away the lift is from the target
	double integral = integral + error;//running sum of the previous errors
	if(error == 0){// makes it so the speed reaches zero when error equals zero
		integral = 0;
	}else{}
	if(abs(integral) >= integral_max_value){//makes sure integral doesnt go farther than need be
		integral = integral_max_value;
	}else{}
	double previous_error = error;
	double derivative = error - previous_error;
	double speed = (Kp*error) + (Ki*integral) + (Kd*derivative);// constantly slowing speed
	if(master.get_digital(DIGITAL_B)){
 		lift.move(speed);
 }else{
 	lift.move_velocity(0);
 }
 }
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


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	BackBlue();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {//An infinite while loop
    //tankDrive();
    arcadeDrive();//calling the functions
		Lift();
		Angler();
		Intake();
    pros::delay(10);//delay of ten milliseconds, prevents the code from crashing
  }
}
