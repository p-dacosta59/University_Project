#include "DriverAuto.h"

////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*####****************** CONSTRUCTOR ******************####*/
DriverAuto::DriverAuto(){
   this->fd =  serialOpen("/dev/ttyUSB0", 300);
   this->initialise();
   this->calibrage = 1;
}

/*####****************** DRIVE AUTO ******************####*/
void DriverAuto::driveAuto(Poco::Data::Session* pointerSesion){
   if(this->getPosition(pointerSesion)){
      // DECL
      this->closerUserTag(pointerSesion);
      int size(this->mapBalise.size());
      for(int i(0); i < size; i++){
         deque<FindTag> tabWithPath(this->searchShortestTag());
         Poco::Data::Statement insert(*pointerSesion);
         Poco::Data::Statement update(*pointerSesion);
         Poco::Data::Statement erase(*pointerSesion);
         deque<FindTag>::iterator itBegin = tabWithPath.begin();
         int posX(0);
         int posY(0);
         int lap(0);
         if(itBegin != tabWithPath.end()){
            erase << "DELETE FROM map WHERE type = 'path' OR type = 'final';";
            erase.execute();
            // Update DB with the next balise
            posX =  this->nextBalise.posX;
            posY = this->nextBalise.posY;
            update << "UPDATE map SET type = 'final' WHERE positionX = ? AND positionY = ?;",
               Poco::Data::Keywords::use(posX),
               Poco::Data::Keywords::use(posY);
            update.execute();
            // Insert in db the path used
            insert << "INSERT INTO map VALUES('path', ?, ?, ?)",
               Poco::Data::Keywords::use(lap),
               Poco::Data::Keywords::use(posX),
               Poco::Data::Keywords::use(posY);
            for (itBegin ;itBegin != tabWithPath.end(); itBegin++){
               posX =  itBegin->posX;
               posY = itBegin->posY;
               insert.execute();
               lap++;
            }
         }
         itBegin = tabWithPath.begin();
         while(itBegin != tabWithPath.end()){
            // Drive the car using the path defined
            deque<FindTag>::iterator itEnd = find_if(itBegin, tabWithPath.end(), FindVecteurNextTag(itBegin->posX, itBegin->posY));
            if(itEnd != tabWithPath.end()){
               if(itEnd->posX == myPos.posX){
                  if((itEnd->posY - myPos.posY) > 0)
                     this->turn90Left();
                  else
                     this->turn90Rigth();
                  this->rollInFront(abs(itEnd->posX - itBegin->posX));
               }
               else if(itEnd->posY == myPos.posY){
                  if((itEnd->posX - myPos.posX) > 0)
                     this->turn90Left();
                  else
                     this->turn90Rigth();
                  this->rollInFront(abs(itEnd->posY - itBegin->posY));
               }
            }else{
               this->goBack(abs(itEnd->posY - itBegin->posY));
            }
            if(!this->getPosition(pointerSesion))
               return;
            if(myPos.posX = itEnd->posX && myPos.posY == itEnd->posY)
               itBegin = itEnd;
         }
         this->mapBalise.pop();
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

/*####****************** INITIALISE VAR ******************####*/
void DriverAuto::initialise(){
   this->myPos.posX = 0;
   this->myPos.posY = 0;
   this->tabMap.clear();
}