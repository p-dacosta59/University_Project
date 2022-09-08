// ////////////////////////////////////////////////////////////////////////////////
// // HEAD
#include "utils.h"
#include "DoOneStep.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// BODY
int main(){
	// Begin
	cout << "[BEGIN]-------------------------------------------\n";
	//Initialisation wtiring PI
	if(wiringPiSetup()<0) 
		return 1;

	// DECL
	bool mainLoop(true);
	DoOneStep mainStep(&mainLoop);
	
	//Main Loop
	while(mainLoop){
		mainStep.doOneStep();
	}

	// End
	cout << "[END]---------------------------------------------\n";
	return 0;
}

