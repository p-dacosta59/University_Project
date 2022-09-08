#include "Moteur.h"

////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

/*####****************** CONSTRUCTOR ******************####*/
Moteur::Moteur():speedMotor1(0), speedMotor2(0) {
	for (int pin = 0 ; pin < 32 ; ++pin){
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}
	pinMode(MOTOR1_EN, PWM_OUTPUT);
	pwmWrite(MOTOR1_EN, 0);
	pinMode(MOTOR2_EN, PWM_OUTPUT);
	pwmWrite(MOTOR2_EN, 0);
}


/*####****************** CONTROLL ALL MOTORS ******************####*/
void Moteur::centerControle(){
	if(this->speedMotor1 == 0 && this->speedMotor2 == 0) 
		this->stop();
	if(this->speedMotor1 >= 0) 
		this->motor1Front();
	else 
		this->motor1Back();
	if(this->speedMotor2 >= 0) 
		this->motor2Front();
	else 
		this->motor2Back();
}

/*####****************** ADD SPEED ******************####*/
void Moteur::addSpeedMotor1(int i){
	this->speedMotor1 += i;
	if(this->speedMotor1 > 100) this->speedMotor1 = 100;
}
void Moteur::addSpeedMotor2(int i){
	this->speedMotor2 += i;
	if(this->speedMotor2 > 100) this->speedMotor2 = 100;
}

/*####****************** LESS SPEED ******************####*/
void Moteur::lessSpeedMotor1(int i){
	this->speedMotor1 -= i;
	if(this->speedMotor1 < -100) this->speedMotor1 = -100;
}
void Moteur::lessSpeedMotor2(int i){
	this->speedMotor2 -= i;
	if(this->speedMotor2 < -100) this->speedMotor2 = -100;
}

/*####****************** SLOW DOWN ******************####*/
void Moteur::slowDown(){
	if(this->speedMotor1 <= 10 && this->speedMotor1 >= -10) this->speedMotor1 = 0;
	else if(this->speedMotor1 > 10) this->speedMotor1 -= 10;
	else if(this->speedMotor1 < -10) this->speedMotor1 += 10;

	if(this->speedMotor2 <= 10 && this->speedMotor2 >= -10) this->speedMotor2 = 0;
	else if(this->speedMotor2 > 10) this->speedMotor2 -= 10;
	else if(this->speedMotor2 < -10) this->speedMotor2 += 10;
}

/*####****************** STOP MOTORS ******************####*/
void Moteur::stop(){
	pwmWrite(MOTOR1_EN, 0);
	digitalWrite(MOTOR1_A, LOW);
	digitalWrite(MOTOR1_B, LOW);
	
	pwmWrite(MOTOR2_EN, 0);
	digitalWrite(MOTOR2_A, LOW);
	digitalWrite(MOTOR2_B, LOW);
	this->speedMotor1 = 0;
	this->speedMotor2 = 0;
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
/*####****************** SEND INTRUCTION TO MOTOR ******************####*/
void Moteur::motor1Front(){
    int speed(abs((1024/100)*this->speedMotor1));
	pwmWrite(MOTOR1_EN, speed);	
	digitalWrite(MOTOR1_A, HIGH);
	digitalWrite(MOTOR1_B, LOW);
}
void Moteur::motor1Back(){
	int speed(abs((1024/100)*this->speedMotor1));
	pwmWrite(MOTOR1_EN, speed);	
	digitalWrite(MOTOR1_A, LOW);
	digitalWrite(MOTOR1_B, HIGH);
}
void Moteur::motor2Front(){
    int speed(abs((1024/100)*this->speedMotor2));
	pwmWrite(MOTOR2_EN, speed);
	digitalWrite(MOTOR2_A, HIGH);
	digitalWrite(MOTOR2_B, LOW);
}
void Moteur::motor2Back(){
    int speed(abs((1024/100)*this->speedMotor2));
	pwmWrite(MOTOR2_EN, speed);
	digitalWrite(MOTOR2_A, LOW);
	digitalWrite(MOTOR2_B, HIGH);
}