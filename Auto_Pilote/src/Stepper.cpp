#include "Stepper.h"

////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*####****************** CONSTRUCTOR ******************####*/
Stepper::Stepper():rolls(0) {
    pinMode(IN1, OUTPUT); 
    pinMode(IN2, OUTPUT); 
    pinMode(IN3, OUTPUT); 
    pinMode(IN4, OUTPUT); 
}

/*####****************** RULL THE MOTOR STEP BY STEP ******************####*/
void Stepper::stepper(int nbStep){
    int steps = 0;
    bool direction = false;
    this->rolls += nbStep;
    if(nbStep>=0) 
        direction = true;
    else{
        direction = false;
        nbStep = -nbStep;
    }
    for (int x(0); x < nbStep*8; x++){
        switch(steps){
            case 0:
                digitalWrite(IN1, LOW); 
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, HIGH);
                break; 
            case 1:
                digitalWrite(IN1, LOW); 
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, HIGH);
                break; 
            case 2:
                digitalWrite(IN1, LOW); 
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, LOW);
                break; 
            case 3:
                digitalWrite(IN1, LOW); 
                digitalWrite(IN2, HIGH);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, LOW);
                break; 
            case 4:
                digitalWrite(IN1, LOW); 
                digitalWrite(IN2, HIGH);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                break; 
            case 5:
                digitalWrite(IN1, HIGH); 
                digitalWrite(IN2, HIGH);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                break; 
            case 6:
                digitalWrite(IN1, HIGH); 
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                break; 
            case 7:
                digitalWrite(IN1, HIGH); 
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, HIGH);
                break; 
            default:
                digitalWrite(IN1, LOW); 
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                break; 
        }
        delayMicroseconds(1000);
        if(direction) 
            steps++;
        else 
            steps--; 
        if(steps>7) 
            steps=0;
        if(steps<0) 
            steps=7;
    } 
}

/*####****************** UNROLL CABLE ******************####*/
void Stepper::unrolls(){
    this->stepper(-this->rolls);
    this->rolls = 0;
}

/*####****************** STOP RULLS ******************####*/
void Stepper::stopRulls(){
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
