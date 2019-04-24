

#define VALVE_CONTROL_PIN 4
#define VALVE_TRIGGER_NOW_PIN 5
#define VALVE_TRIGGER_LATER_PIN 6

#define VALVE_DURATION_PIN_0 7
#define VALVE_DURATION_PIN_1 8
#define VALVE_DURATION_PIN_2 9

#define DURATION 8000    // eight seconds on
#define INTERVAL 180000 //  every three minutes
#define MAX_INTERVAL 600000 //  every 10 minutes

/*
 * STATE FUNCTION PROTOTYPES AND VARIABLES
 */
unsigned long nextTriggerTime = 0;
unsigned long lastTriggerTime = 0;
unsigned long duration = DURATION;
void initialTaskStartFunction();
void timerTaskStartFunction();
void timerTaskEndFunction();
void externalInputTask();
void (*currentTaskFunction)(void) = nilTaskFunction;



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(VALVE_CONTROL_PIN, OUTPUT);
  pinMode(VALVE_TRIGGER_NOW_PIN, INPUT);
  pinMode(VALVE_TRIGGER_LATER_PIN, INPUT);
  pinMode(VALVE_DURATION_PIN_0, INPUT);
  pinMode(VALVE_DURATION_PIN_1, INPUT);
  pinMode(VALVE_DURATION_PIN_2, INPUT);

  nextTriggerTime = 0;
  duration = DURATION;
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
    digitalWrite(VALVE_CONTROL_PIN, HIGH);
    lastTriggerTime = millis();
    nextTriggerTime = lastTriggerTime + 10000; //turn off in ten seconds
    currentTaskFunction = timerTaskEndFunction;
  }
}

void timerTaskStartFunction() {
  if( millis() >= nextTriggerTime ) {
    digitalWrite(VALVE_CONTROL_PIN, HIGH);
    lastTriggerTime = millis();
    nextTriggerTime = lastTriggerTime + duration; //turn on for duration
    currentTaskFunction = timerTaskEndFunction;
  }
}

void timerTaskEndFunction() {
  unsigned long now = millis();
  if( now >= (lastTriggerTime + duration) ) {
    digitalWrite(VALVE_CONTROL_PIN, LOW);
    nextTriggerTime = millis() + INTERVAL; //turn off for interval
    currentTaskFunction = timerTaskStartFunction;
  }
}

void externalInputTask() {
  if( digitalRead(VALVE_TRIGGER_NOW_PIN) == HIGH ) {
    // check to see if the trigger now pin has been set by an external source
    // set trigger time and state to fire righ away
    nextTriggerTime = 0;
    currentTaskFunction = timerTaskStartFunction;
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







