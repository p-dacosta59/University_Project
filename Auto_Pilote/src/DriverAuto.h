#ifndef _DRIVER_AUTO_H
#define _DRIVER_AUTO_H

////////////////////////////////////////////////////////////////////////////////
// HEAD
#include "Driver.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// CLASS
class DriverAuto : public Driver {
	//////////////////////////////
	// PUBLIC METHODS
	public:
	/*####****************** CONSTRUCTOR ******************####*/
	DriverAuto();

	/*####****************** DRIVE AUTO ******************####*/
	void driveAuto(Poco::Data::Session* pointerSesion);
	
	//////////////////////////////
	// PRIVATE METHODS
	private:
	/*####****************** INITIALISE VAR ******************####*/
	void initialise();
};

#endif