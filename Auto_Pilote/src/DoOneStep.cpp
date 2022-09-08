#include "DoOneStep.h"

////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*####****************** CONSTRUCTOR ******************####*/
DoOneStep::DoOneStep(bool *boolMain):mainLoop(boolMain),loopPicture(true) {
   Poco::Data::MySQL::Connector::registerConnector();
   this->pictureThread = thread(takePictures, &loopPicture);
   // DB Connection
   this->session = new Poco::Data::Session(Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, "host=127.0.0.1;user=root;password=root;db=projetTut;compress=true;auto-reconnect=true"));
   Poco::Data::Statement update(*this->session);
   update << "UPDATE vehicule SET powerOn = True, move = FALSE, ready = FALSE";
   update.execute();
   if(this->autoDriver.getPosition(this->session))
      this->autoDriver.calibration();
}

/*####****************** DESTRUCTOR ******************####*/
DoOneStep::~DoOneStep() {
   delete this->session;
}

/*####****************** DO ONE STEP ******************####*/
void DoOneStep::doOneStep(){
   //Analise of Data
   this->analiseData();
}

/*####****************** STOP STEP ******************####*/
void DoOneStep::stopStep(){
   this->loopPicture = false;
   this->pictureThread.join();
   *this->mainLoop = false;
   Poco::Data::Statement update(*this->session);
   update << "UPDATE vehicule SET powerOn = FALSE, move = FALSE, ready = FALSE";
   update.execute();
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/*####****************** ANALISE DATA RECEIVED ******************####*/
void DoOneStep::analiseData(){
   Poco::Data::Statement select(*this->session);
	bool ready(false);
	bool powerOn(false);
   select << "SELECT ready, powerOn FROM vehicule",
      Poco::Data::Keywords::into(ready),
      Poco::Data::Keywords::into(powerOn),
      Poco::Data::Keywords::now;
	if(!powerOn)
      this->stopStep();
   else if(ready){
      Poco::Data::Statement update(*this->session);
      update << "UPDATE vehicule SET move = TRUE";
      update.execute();
      this->autoDriver.driveAuto(this->session);
      Poco::Data::Statement update2(*this->session);
      update2 << "UPDATE vehicule SET move = FALSE, ready = FALSE";
      update2.execute();
   }
   else
      Utils::delay(1000);
}