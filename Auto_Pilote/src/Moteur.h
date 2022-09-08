#ifndef MOTEUR__H
#define MOTEUR__H

////////////////////////////////////////////////////////////////////////////////
// HEAD
#include <iostream>
#include <math.h>
#include <wiringPi.h>

#define MOTOR1_EN 1
#define MOTOR1_A 5
#define MOTOR1_B 4

#define MOTOR2_EN 26
#define MOTOR2_A 27
#define MOTOR2_B 28

using namespace std;



////////////////////////////////////////////////////////////////////////////////
// CLASS
class Moteur{
	//////////////////////////////
	// MEMBERS
	private:
	int speedMotor1;
	int speedMotor2;

	//////////////////////////////
	// PUBLIC METHODS
	public:
	/*####****************** CONSTRUCTOR ******************####*/
	Moteur();

	/*####****************** CONTROLL ALL MOTORS ******************####*/
	void centerControle();

	/*####****************** ADD SPEED ******************####*/
	void addSpeedMotor1(int i = 10);
	void addSpeedMotor2(int i = 10);

	/*####****************** LESS SPEED ******************####*/
	void lessSpeedMotor1(int i = 10);
	void lessSpeedMotor2(int i = 10);

	/*####****************** SLOW DOWN ******************####*/
	void slowDown();

	/*####****************** STOP MOTORS ******************####*/
	void stop();

	//////////////////////////////
	// PRIVATE METHODS
	private:
	/*####****************** SEND INTRUCTION TO MOTOR ******************####*/
	void motor1Front();
	void motor1Back();
	void motor2Front();
	void motor2Back();
};
#endif
