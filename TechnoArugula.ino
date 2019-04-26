
#include "CO2Monitor.h"
#include "SprayHandler.h"


#define VALVE_1_CONTROL_PIN 2
#define VALVE_2_CONTROL_PIN 3
#define VALVE_3_CONTROL_PIN 4

#define VALVE_TRIGGER_NOW_PIN 5
#define VALVE_TRIGGER_LATER_PIN 6

#define VALVE_DURATION_PIN_0 7
#define VALVE_DURATION_PIN_1 8
#define VALVE_DURATION_PIN_2 9


SprayHandler sprayTask(VALVE_1_CONTROL_PIN, VALVE_2_CONTROL_PIN, VALVE_3_CONTROL_PIN);


// the setup function runs once when you press reset or power the board
void setup() {
//  Serial.begin(115200); //start a serial port to communicate with the computer
//  Serial.println();

  sprayTask.initialize();
}

void loop() {
  sprayTask.perform();
}
