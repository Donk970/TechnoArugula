

#define VALVE_1_CONTROL_PIN 2
#define VALVE_2_CONTROL_PIN 3
#define VALVE_3_CONTROL_PIN 4

#define VALVE_TRIGGER_NOW_PIN 5
#define VALVE_TRIGGER_LATER_PIN 6

#define VALVE_DURATION_PIN_0 7
#define VALVE_DURATION_PIN_1 8
#define VALVE_DURATION_PIN_2 9

#define STARTUP_DURATION 10000    // ten seconds on
#define DURATION 500    // half second on
#define INTERVAL 300000 //  every five minutes
#define MAX_INTERVAL 600000 //  every 10 minutes

/*
 * STATE FUNCTION PROTOTYPES AND VARIABLES
 */
unsigned long nextTriggerTime = 0;
unsigned long lastTriggerTime = 0;
unsigned long duration = DURATION;

void initialTaskStartFunction();

// start stop for valve 1
void timerTask1StartFunction();
void timerTask1EndFunction();

// start stop for valve 2
void timerTask2StartFunction();
void timerTask2EndFunction();

// start stop for valve 3
void timerTask3StartFunction();
void timerTask3EndFunction();


void externalInputTask();
void (*currentTaskFunction)(void) = initialTaskStartFunction;



// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(VALVE_1_CONTROL_PIN, OUTPUT);
  pinMode(VALVE_2_CONTROL_PIN, OUTPUT);
  pinMode(VALVE_3_CONTROL_PIN, OUTPUT);
  
  pinMode(VALVE_TRIGGER_NOW_PIN, INPUT);
  pinMode(VALVE_TRIGGER_LATER_PIN, INPUT);
  pinMode(VALVE_DURATION_PIN_0, INPUT);
  pinMode(VALVE_DURATION_PIN_1, INPUT);
  pinMode(VALVE_DURATION_PIN_2, INPUT);

  nextTriggerTime = 0;
  duration = STARTUP_DURATION;
  currentTaskFunction = initialTaskStartFunction;
}

void loop() {
  // check external inputs to determin if normal timing shoud be adjusted
  externalInputTask();
  
  // execute the current state task
  currentTaskFunction();
}





/*
 * STATE FUNCTIONS
 */
void initialTaskStartFunction() {  
  if( millis() >= nextTriggerTime ) {
    digitalWrite(VALVE_1_CONTROL_PIN, HIGH);
    duration = STARTUP_DURATION; //set long duration
    lastTriggerTime = 0;
    nextTriggerTime = 0;
    currentTaskFunction = timerTask1StartFunction;
  }
}


//---------------------------------------------------------------------------
// start stop for valve 1
//---------------------------------------------------------------------------
void timerTask1StartFunction() {
  if( millis() >= nextTriggerTime ) {
    digitalWrite(VALVE_1_CONTROL_PIN, HIGH);
    lastTriggerTime = millis();
    nextTriggerTime = lastTriggerTime + duration; //turn on for duration
    currentTaskFunction = timerTask1EndFunction;
  }
}

void timerTask1EndFunction() {
  unsigned long now = millis();
  if( now >= (lastTriggerTime + duration) ) {
    digitalWrite(VALVE_1_CONTROL_PIN, LOW);
    nextTriggerTime = millis() + 1000; //turn off for 1 second then do valve 2
    currentTaskFunction = timerTask2StartFunction;
  }
}



//---------------------------------------------------------------------------
// start stop for valve 2
//---------------------------------------------------------------------------
void timerTask2StartFunction() {
  if( millis() >= nextTriggerTime ) {
    digitalWrite(VALVE_2_CONTROL_PIN, HIGH);
    lastTriggerTime = millis();
    nextTriggerTime = lastTriggerTime + duration; //turn on for duration
    currentTaskFunction = timerTask2EndFunction;
  }
}

void timerTask2EndFunction() {
  unsigned long now = millis();
  if( now >= (lastTriggerTime + duration) ) {
    digitalWrite(VALVE_2_CONTROL_PIN, LOW);
    nextTriggerTime = millis() + 1000; //turn off for 1 second then do valve 3
    currentTaskFunction = timerTask3StartFunction;
  }
}



//---------------------------------------------------------------------------
// start stop for valve 3
//---------------------------------------------------------------------------
void timerTask3StartFunction() {
  if( millis() >= nextTriggerTime ) {
    digitalWrite(VALVE_3_CONTROL_PIN, HIGH);
    lastTriggerTime = millis();
    nextTriggerTime = lastTriggerTime + duration; //turn on for duration
    currentTaskFunction = timerTask3EndFunction;
  }
}

void timerTask3EndFunction() {
  unsigned long now = millis();
  if( now >= (lastTriggerTime + duration) ) {
    digitalWrite(VALVE_3_CONTROL_PIN, LOW);
    nextTriggerTime = millis() + INTERVAL; //turn off for interval before doing valve 1 again
    currentTaskFunction = timerTask1StartFunction;
    duration = DURATION;  // reset to normal duration
  }
}




//---------------------------------------------------------------------------
// the external input task
//---------------------------------------------------------------------------
void externalInputTask() {
  if( digitalRead(VALVE_TRIGGER_NOW_PIN) == HIGH ) {
    // check to see if the trigger now pin has been set by an external source
    // set trigger time and state to fire righ away
    nextTriggerTime = 0;
    currentTaskFunction = timerTask1StartFunction;
  } else if( digitalRead(VALVE_TRIGGER_LATER_PIN) == HIGH ) {
    // check to see if the trigger later pin has been set by an external source
    if( (nextTriggerTime - lastTriggerTime) < (MAX_INTERVAL - 10000) ) {
      // if we havent reached our maximum interval, add ten seconds
      nextTriggerTime += 10000;
    }
  }

  // read valve duration pins
  unsigned long value = 0;
  if( digitalRead(VALVE_DURATION_PIN_0) == HIGH ) { value = 1; }
  if( digitalRead(VALVE_DURATION_PIN_1) == HIGH ) { value += 1 << 1; }
  if( digitalRead(VALVE_DURATION_PIN_2) == HIGH ) { value += 1 << 2; }
  duration = DURATION - value;
  duration = duration > 1 ? (duration < DURATION ? duration : DURATION) : 1;
}
