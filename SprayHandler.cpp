

#include "SprayHandler.h"


SprayValve :: SprayValve() {
  this->duration = 500;
  this->interval = 60000; // default interval until next valve should be set is one minute
}

void SprayValve :: initialize() {
  pinMode(this->pin, OUTPUT);
  this->resetValve(true);
}

void SprayValve :: resetValve( bool triggerLongSpray = false ) {
  this->dt = 0;
  this->done = false;
  this->state = &SprayValve::openValve;
  this->triggerLongSpray = triggerLongSpray;
}

void SprayValve :: startValve() {
  this->dt = millis() + this->interval;
}



SprayValve* SprayValve :: perform() {
  (this->*state)();
  if( done ) {
    Serial.println("------------------   NEXT VALVE  -------------------");

    // make sure next valves dt is correctly set
    this->nextValve->startValve();

    // return the next valve
    return this->nextValve;
  }
  return this;
}

void SprayValve :: waitingToOpenValve() {
  this->done = false;
  if( millis() > this->dt ) {
    Serial.println("waitingToOpenValve -> openValve");
    this->state = &SprayValve::openValve;
  }
}

void SprayValve :: openValve() {
  this->done = false;
  this->dt = this->triggerLongSpray ? millis() + 20000 : millis() + this->duration;
  this->triggerLongSpray = false;
  this->state = &SprayValve::valveOpen;
  Serial.println("openValve -> valveOpen");
  digitalWrite(this->pin, HIGH);
}

void SprayValve :: valveOpen() {
  this->done = false;
  if( millis() > this->dt ) {
    this->dt = millis() + this->interval;
    Serial.println("valveOpen -> waitingToOpenValve");
    this->state = &SprayValve::waitingToOpenValve;
    digitalWrite(this->pin, LOW);
    this->done = true;
    this->triggerLongSpray = false;
  }
}







SprayHandler :: SprayHandler(uint8_t valveAPin, uint8_t valveBPin, uint8_t valveCPin) {
  //construct the valve network
  this->valveA.pin = valveAPin;
  this->valveA.nextValve = &(this->valveB);
  this->valveA.interval = 900000; //interval until valveA opens is 15 minutes
  
  this->valveB.pin = valveBPin;
  this->valveB.nextValve = &(this->valveC);
  
  this->valveC.pin = valveCPin;
  this->valveC.nextValve = &(this->valveA);
  
  this->currentValve = &(this->valveA);
}


void SprayHandler :: initialize() {
  this->valveA.initialize();
  this->valveB.initialize();
  this->valveC.initialize();
}

void SprayHandler :: perform() {
  this->currentValve =  this->currentValve->perform();
}

void SprayHandler :: triggerLongSpray() {
  this->valveA.resetValve(true);
  this->valveB.resetValve(true);
  this->valveC.resetValve(true);  
  this->currentValve = &(this->valveA);
}
