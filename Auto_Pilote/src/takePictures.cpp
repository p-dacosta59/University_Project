#include "takePictures.h"

////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
/*####****************** LOOP TO TAKE THE PICTURES AND SAVE IT ******************####*/
void takePictures(bool *loop){
	int i(0);
	while(*loop){
		string cmd("raspistill -t 1 -w 640 -h 480 -q 100 -o /var/www/html/projet/img" + Utils::toString(i) + ".png");
		system(cmd.c_str());
		if(i) i = 0;
		else i = 1;
	}
}