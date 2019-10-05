#include "main.h"
#include "main.h"
#define LEFT_FRONT 12 //Sets a variable that holds the port value(allows for easy port changes)
#define RIGHT_FRONT 19
#define LEFT_BACK 13
#define RIGHT_BACK 18
#define LIFT 14
#define ANGLER 15
#define INTAKELEFT 16
#define INTAKERIGHT 17

pros::Motor Lf (LEFT_FRONT);//Lets the motor name and sets the port
pros::Motor Rf (RIGHT_FRONT, true); // This reverses the motor
pros::Motor Lb (LEFT_BACK);
pros::Motor Rb (RIGHT_BACK, true);
pros::Motor lift (LIFT);
pros::Motor angler (ANGLER);
pros::Motor intakeL (INTAKELEFT);
pros::Motor intakeR (INTAKERIGHT, true);

#include "okapi/api.hpp"
using namespace okapi;
using namespace okapi::literals;

okapi::MotorGroup Intake({16, -17});

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
 	profileController.setTarget("m");
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
  Intake.moveVelocity(200);
  move(Point{4.2_ft, 0_ft, 0_deg});
  drive.turnAngle(35_deg);
  lift.move_velocity(200);
  pros::delay(1500);
  Intake.moveVelocity(-200);
  pros::delay(1000);
  Intake.moveVelocity(0);
  lift.move_velocity(-200);
  pros::delay(1500);
  Intake.moveVelocity(200);
  drive.turnAngle(100_deg);
  move(Point{4.1_ft, 0_ft, 0_deg});
  move(Point{1_ft, 0_ft,0_deg});
  angler.move_velocity(80);
  pros::delay(1500);
  moveb(Point{1_ft,0_ft,0_deg});




}

void BackRed(){}

void autonomous() {
 BackBlue();
}
