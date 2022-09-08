#ifndef _DOONESTEP_H
#define _DOONESTEP_H

////////////////////////////////////////////////////////////////////////////////
// HEAD
#include <thread>
#include "Poco/Data/MySQL/Connector.h"

#include "takePictures.h"
#include "DriverAuto.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// CLASS
class DoOneStep {
	//////////////////////////////
	// MEMBERS
	private:
	DriverAuto autoDriver;
	Poco::Data::Session *session;
	thread pictureThread;
	bool loopPicture;
	bool *mainLoop;

	//////////////////////////////
	// PUBLIC METHODS
	public:
	/*####****************** CONSTRUCTOR ******************####*/
	DoOneStep(bool *boolMain);
	
	/*####****************** DESTRUCTOR ******************####*/
	~DoOneStep();
	
	/*####****************** DO ONE STEP ******************####*/
	void doOneStep();
	
	/*####****************** STOP STEP ******************####*/
	void stopStep();
	
	//////////////////////////////
	// PRIVATE METHODS
	private:
	/*####****************** ANALISE DATA RECEIVED ******************####*/
	void analiseData();
};

#endif