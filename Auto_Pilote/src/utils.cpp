#include "utils.h"

////////////////////////////////////////////////////////////////////////////////
// METHODS

//// Random numbers ////
// Init random generator
void Utils::initRandomGenerator(int seed) {
	if (seed == 0) {
		srand(time(NULL));
	} else {
		srand(seed);
	}
}

// Get random integer in interval [min ; max]
int Utils::getRandom(int min, int max) {
	return rand()%(max-min+1) + min;
}

//// Conversions ////
// Number to string
string Utils::toString(int nb) {
	std::ostringstream ss;
  ss << nb;
  return ss.str();
}
string Utils::toString(double nb) {
	std::ostringstream ss;
  ss << nb;
  return ss.str();
}
string Utils::toString(bool nb) {
	std::ostringstream ss;
  ss << nb;
  return ss.str();
}

// String to number 
int Utils::toInt(string nbString){
    int nb(0);
    char data;
    for (int i=0; i<nbString.size(); i++){
        data = nbString[i];
        int nbFor(0);
        switch (data) {
            case 48:
                nbFor = 0;
                break;
            case 49:
                nbFor = 1;
                break;
            case 50:
                nbFor = 2;
                break;
            case 51:
                nbFor = 3;
                break;
            case 52:
                nbFor = 4;
                break;
            case 53:
                nbFor = 5;
                break;
            case 54:
                nbFor = 6;
                break;
            case 55:
                nbFor = 7;
                break;
            case 56:
                nbFor = 8;
                break;
            case 57:
                nbFor = 9;
                break;
        }
        nb += nbFor * pow(10, (nbString.size()-1-i));
    }
    return nb;
}
// String to string
string Utils::toLower(string str) {
	for (int i=0 ; i<str.length() ; i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}
string Utils::toUpper(string str) {
	for (int i=0 ; i<str.length() ; i++) {
		str[i] = toupper(str[i]);
	}
	return str;
}
string Utils::toFirstName(string str) {
	if (str.length() > 0) str[0] = toupper(str[0]);
	for (int i=1 ; i<str.length() ; i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}

// Number to character (0 -> A, 1 -> B, 2 -> C, ...)
string Utils::toCharacter(int nb) {
	std::ostringstream ss;
  	ss << (char)(65 + nb);
  	return ss.str();
}

// Character to number (A -> 0, B -> 1, C -> 2, ...)
int Utils::toNumber(char character) {
	if (character >= 65 && character < 91) {
		return character - 65;
	} else if (character >= 48 && character < 58) {
		return character - 48;
	} else if (character >= 97 && character < 123) {
		return character - 97;
	} else {
		return -1;
	}
}

//// Screen ////
// Clear screen
void Utils::clearScreen() {
	for (int i=0 ; i<100 ; i++) {
		cout << "\n";
	}
}
// Delay screen (time in ms)
void Utils::delay(int timeMs) {
	usleep((long)1000 * (long)timeMs);
}

