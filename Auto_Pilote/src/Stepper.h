#ifndef _STEPPER_H
#define _STEPPER_H

////////////////////////////////////////////////////////////////////////////////
// HEAD

#include "utils.h"
#include <wiringPi.h>
using namespace std;

#define IN1 21
#define IN2 22
#define IN3 23
#define IN4 24

////////////////////////////////////////////////////////////////////////////////
// CLASS
class Stepper {

	//////////////////////////////
	// MEMBERS
	private:
	int rolls;

	//////////////////////////////
	// PUBLIC METHODS
	public:
	/*####****************** CONSTRUCTOR ******************####*/
	Stepper();

	/*####****************** RULL THE MOTOR STEP BY STEP ******************####*/
	void stepper(int nbStep = 512);
	
	/*####****************** UNROLL CABLE ******************####*/
	void unrolls();

	/*####****************** STOP RULLS ******************####*/
	void stopRulls();
};

#endif