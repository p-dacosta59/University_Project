#ifndef _DRIVER_H
#define _DRIVER_H

////////////////////////////////////////////////////////////////////////////////
// HEAD

#include <wiringSerial.h>
#include <vector>
#include <deque>
#include <queue> 
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/MySQL/MySQLException.h"

#include "Moteur.h"
#include "Balise.h"
#include "Stepper.h"

#define SIZE_TAB 49
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// CLASS
class Driver {

	//////////////////////////////
	// MEMBERS
	protected:
	Moteur piloteCar;
	Tag myPos;
	Tag nextBalise;
	Stepper motor;
	queue<Tag> mapBalise;
	vector<Tag> mapInterdiction;
	vector <PositionTag> tabMap;
	int fd;
	double calibrage;

	//////////////////////////////
	// PUBLIC METHODS
	public:
	/*####****************** LOOP STEP ******************####*/
	void doOneStep();

	/*####****************** STOP DIRVE ******************####*/
	void stopped();

	/*####****************** GET MY POSITION BY A TRIANGULATION ******************####*/
	bool getPosition(Poco::Data::Session* pointerSesion);

	/*####****************** GET DEGREE OF TAG ******************####*/
	void getPositionOfTag();

	/*####****************** SEARCH IN DATABASE WHAT TAG CLOSER ******************####*/
	void closerUserTag(Poco::Data::Session* pointerSesion);
	
	/*####****************** CALIBRATION OF THE CAR'S ENGINES FOR SPEED AND TURN ******************####*/
	void calibration();
	//////////////////////////////
	// PROTECTED METHODS
	protected:

	/*####****************** SEARCH SHORTEST TAG IN MAP ******************####*/
	deque<FindTag> searchShortestTag();

	/*####****************** READ DATA OF ARDUINO ******************####*/
	string readDataArduino();
	
	/*####****************** CLEAR DATA SERIAL ******************####*/
	void clearDataArduino();

	/*####****************** TURN THE CAR WITH DEG ******************####*/
	void turn90Left();
	
	void turn90Rigth();

	void turn180();

	/*####****************** GO BACK IN MY PATH ******************####*/
	void goBack(int length=1);

	/*####****************** ROLL IN FRONT ******************####*/
	void rollInFront(int length=1);

	//////////////////////////////
	// PRIVATE METHODS
	private:
	/*####****************** CHECK IF BALISE ARE IN MAP ******************####*/
	bool isValid(int row, int col);

	/*####****************** GET THE DISTANCE WITH 2 POINT ******************####*/
	double distance(int &posX1, int &posY1, int &posX2, int &posY2);

};

#endif