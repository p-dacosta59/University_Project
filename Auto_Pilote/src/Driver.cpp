#include "Driver.h"

////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*####****************** LOOP STEP ******************####*/
void Driver::doOneStep(){
	this->piloteCar.centerControle();
}

/*####****************** STOP DIRVE ******************####*/
void Driver::stopped(){
   this->piloteCar.stop();
   serialClose(this->fd);
}

/*####****************** GET MY POSITION BY A TRIANGULATION ******************####*/
bool Driver::getPosition(Poco::Data::Session* pointerSesion){
   // Receive the target name and Angle
   this->getPositionOfTag();
   if(this->tabMap.size() == 4){
      Poco::Data::Statement select(*pointerSesion);
      string tagName;
      int posX(0);
      int posY(0);
      double difference(1000);
      bool killBoucle(false);
      // Search in database the target position
      select << "SELECT positionX, positionY FROM map WHERE type = '?'",
         Poco::Data::Keywords::into(posX),
         Poco::Data::Keywords::into(posY),
         Poco::Data::Keywords::use(tagName),
         Poco::Data::Keywords::range(0, 1);

      for(vector<PositionTag>::iterator it = this->tabMap.begin(); it != this->tabMap.end(); it++){
         tagName =  "balisePosition" + it->name;
         if(select.done())
            select.execute();
         it->posX = posX;
         it->posY = posY;
      }
      for(int y(0); y < SIZE_TAB; y++){
         for(int x(0); x < SIZE_TAB; x++){
            vector<double> range;
            double deg;
            double diff(0);
            // Calculate my position using 3 target
				for(vector<PositionTag>::iterator it = this->tabMap.begin(); it != this->tabMap.end()-1; it++){
					deg = (acos((pow(distance(it->posX, it->posY, x, y), 2) + pow(distance((it+1)->posX,(it+1)->posY, x, y), 2) - pow(distance(it->posX, it->posY, (it+1)->posX, (it+1)->posY), 2)) / (2 * distance(it->posX, it->posY, x, y) * distance((it+1)->posX, (it+1)->posY, x, y))));
					diff += abs(((it+1)->angle - it->angle)*360/16 - (deg*180/M_PI));
            }
            if(diff == 0){
               this->myPos.posX = x;
               this->myPos.posY = y;
               killBoucle = true;
               break;
            }else if(diff < difference){
               this->myPos.posX = x;
               this->myPos.posY = y;
               difference = diff;
            }
         }
         if(killBoucle)
            break;
      }
      // Position of vehicule finded and updated in db
      Poco::Data::Statement update(*pointerSesion);
      update << "UPDATE vehicule SET posX = ?, posY = ?",
         Poco::Data::Keywords::use(this->myPos.posX),
         Poco::Data::Keywords::use(this->myPos.posY);
      update.execute();
      return true;
   }
   return false;
}

/*####****************** GET DEGREE OF TAG ******************####*/
void Driver::getPositionOfTag(){
   this->tabMap.clear();
   // Use the IR receptor in a motor step by step to search tag
   for(int i(0); i < 16; i++){
      this->motor.stepper(32);
      this->clearDataArduino();
      Utils::delay(1000);
      string data(this->readDataArduino());
      if(data != ""){
         tabMap.push_back(PositionTag(i, data));
         cout << data << endl;
      }
      if(tabMap.size() > 3){
         cout << "ici" << endl;
         break;
      }
   }
   // Return at the origin position for the cable
   this->motor.unrolls();
}

/*####****************** SEARCH IN DATABASE WHAT TAG CLOSER ******************####*/
void Driver::closerUserTag(Poco::Data::Session* pointerSesion){
   Poco::Data::Statement select(*pointerSesion);
   Poco::Data::Statement select2(*pointerSesion);
   std::queue<Tag> empty;
   int posX;
	int posY;
   select << "SELECT positionX, positionY FROM map WHERE type = 'balise' ORDER BY id ASC",
      Poco::Data::Keywords::into(posX),
      Poco::Data::Keywords::into(posY),
      Poco::Data::Keywords::range(0, 1);
   swap(this->mapBalise, empty);
   // Put all target in a table
 	while (!select.done()){
      select.execute();
		this->mapBalise.push(Tag(posX, posY));
   }
   select2 << "SELECT positionX, positionY FROM map WHERE type = 'interdiction' ORDER BY positionX ASC, positionY ASC",
      Poco::Data::Keywords::into(posX),
      Poco::Data::Keywords::into(posY),
      Poco::Data::Keywords::range(0, 1);
   this->mapInterdiction.clear();
 	// Put all interdiction target in a table
   while (!select2.done()){
      select2.execute();
		this->mapInterdiction.push_back(Tag(posX, posY));
   }
}

/*####****************** CALIBRATION OF THE CAR'S ENGINES FOR SPEED AND TURN ******************####*/
void Driver::calibration(){
   int deg(0);
   string data;
   for(int i(0); i < 32; i++){
      this->piloteCar.addSpeedMotor1();
      this->piloteCar.lessSpeedMotor2();
      this->doOneStep();
      Utils::delay(this->calibrage * 1000);
      this->piloteCar.stop();
      this->clearDataArduino();
      Utils::delay(500);
      data = this->readDataArduino();
      if(data != ""){
         deg = i;
         break;
      }
   }

   vector<PositionTag>::iterator it = find_if(this->tabMap.begin(), this->tabMap.end(), FindVecteurName(data));
   this->calibrage = 90 * deg / it->angle;
   // Is used to put the car in rigth position and known the power of my engine
   this->piloteCar.addSpeedMotor1();
   this->piloteCar.lessSpeedMotor2();
   this->doOneStep();
   Utils::delay(it->angle * 1000);
   this->piloteCar.stop();
}

////////////////////////////////////////////////////////////////////////////////
// PROTECTED METHODS
/*####****************** SEARCH SHORTEST TAG IN MAP ******************####*/
// Search why is the Shortest Tag with her path
deque<FindTag> Driver::searchShortestTag(){
   vector<FindTag> q;
	deque<FindTag> tableToReturn;
	int rowNum[] = {-1, 0, 0, 1};
	int colNum[] = {0, -1, 1, 0};
	bool visited[SIZE_TAB][SIZE_TAB];
	memset(visited, false, sizeof visited);
	visited[this->myPos.posX][this->myPos.posY] = true;
   // Browse the map around the car
	q.push_back(FindTag(this->myPos.posX, this->myPos.posY));
	for(int posTab(0); posTab < q.size(); posTab++){
		if (q[posTab].posX == this->mapBalise.front().posX && q[posTab].posY == this->mapBalise.front().posY){
         this->nextBalise.posX = q[posTab].posX;
         this->nextBalise.posY = q[posTab].posX;
			while(posTab != 0){
            tableToReturn.push_front(q[posTab]);
            posTab = q[posTab].previous;
			}
			return tableToReturn;
		}
		for (int i = 0; i < 4; i++){
			int row = q[posTab].posX + rowNum[i];
			int col = q[posTab].posY + colNum[i];
         vector<Tag>::iterator itInterdiction = find_if(this->mapInterdiction.begin(), this->mapInterdiction.end(), FindVecteur(row, col));
			if (isValid(row, col) && itInterdiction == this->mapInterdiction.end() && !visited[row][col]){
            visited[row][col] = true;
				q.push_back(FindTag(row, col, q[posTab].distance + 1, posTab));
			}
		}
	}
	return tableToReturn;
}


/*####****************** READ DATA OF ARDUINO ******************####*/
string Driver::readDataArduino(){
	int thing = 0;
	if(serialDataAvail(this->fd)){
		string data;
		thing = serialGetchar(this->fd);
		data = (char)(thing);
      cout << data << endl;
		if(data != "\n" && data != "" && data != " "){
			cout << data << endl;
			return data;
   	}
   }
   this->clearDataArduino();
   return "";
}	

/*####****************** CLEAR DATA SERIAL ******************####*/
void Driver::clearDataArduino(){
   serialFlush(this->fd);
}

/*####****************** TURN THE CAR WITH DEG ******************####*/
void Driver::turn90Left(){
   this->piloteCar.addSpeedMotor1();
   this->piloteCar.lessSpeedMotor2();
   this->doOneStep();
   Utils::delay(this->calibrage * 1000);
   this->piloteCar.stop();
}
	
void Driver::turn90Rigth(){
   this->piloteCar.addSpeedMotor2();
   this->piloteCar.lessSpeedMotor1();
   this->doOneStep();
   Utils::delay(this->calibrage * 1000);
   this->piloteCar.stop();
}

void Driver::turn180(){
   this->piloteCar.addSpeedMotor1();
   this->piloteCar.lessSpeedMotor2();
   this->doOneStep();
   Utils::delay(2 * this->calibrage * 1000);
   this->piloteCar.stop();
}

/*####****************** GO BACK IN MY PATH ******************####*/
void Driver::goBack(int length){
   this->piloteCar.lessSpeedMotor1();
   this->piloteCar.lessSpeedMotor2();
   this->doOneStep();
   Utils::delay(length * this->calibrage * 1000);
   this->piloteCar.stop();
}

/*####****************** ROLL IN FRONT ******************####*/
void Driver::rollInFront(int length){
   this->piloteCar.addSpeedMotor1();
   this->piloteCar.addSpeedMotor2();
   this->doOneStep();
   Utils::delay(length * this->calibrage * 1000);
   this->piloteCar.stop();
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
/*####****************** CHECK IF BALISE ARE IN MAP ******************####*/
bool Driver::isValid(int row, int col){
	return (row >= 0) && (row < SIZE_TAB) && (col >= 0) && (col < SIZE_TAB);
}

/*####****************** GET THE DISTANCE WITH 2 POINT ******************####*/
double Driver::distance(int &posX1, int &posY1, int &posX2, int &posY2){
   return sqrt(pow((posX1 - posX2), 2) + pow((posY1 - posY2), 2));
}