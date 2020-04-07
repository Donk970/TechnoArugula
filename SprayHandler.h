
#include <Arduino.h>

#ifndef _SPRAY_HANDLER_H_
#define _SPRAY_HANDLER_H_


class SprayValve;
typedef  void (SprayValve::*SprayValveStateFN)(void);

class SprayValve {
  public:
    SprayValve();
    SprayValve *nextValve;
    void initialize();
    void resetValve( bool triggerLongSpray = false );
    void startValve();

    //state functions
    bool done = false;
    SprayValve* perform();
    
    SprayValveStateFN state;
    void waitingToOpenValve();
    void openValve();
    void valveOpen();
    
    uint8_t pin;
    bool triggerLongSpray = true;
    unsigned long dt = 0;
    unsigned long duration; //time valve is open
    unsigned long interval; //time until next valve should open
};



class SprayHandler;
typedef  void (SprayHandler::*SprayHandlerStateFN)(void);


class SprayHandler {
  public:
    SprayHandler(uint8_t valveAPin, uint8_t valveBPin, uint8_t valveCPin);
    void initialize();
    void triggerLongSpray();
    void perform();

  private: 
    SprayValve *currentValve;
    SprayValve valveA;
    SprayValve valveB;
    SprayValve valveC;
};





#endif
