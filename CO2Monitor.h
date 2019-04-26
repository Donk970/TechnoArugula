
#include <Arduino.h>

#ifndef _CO2_MONITOR_H_
#define _CO2_MONITOR_H_


class CO2Monitor;
typedef  void (CO2Monitor::*CO2MonitorStateFN)(void);


class CO2Monitor {
  public:
    CO2Monitor(uint8_t valve);

  private: 
    uint8_t valvePin;

        // state functions
    unsigned long dt = 0;
    CO2MonitorStateFN state;
    void startNewCycle(void);
    void waitForNextReading(void);
    void startReading(void);
    void waitReading(void);
    void finishReading(void);
    void openCO2Valve(void);
    void closeCO2Valve(void);

};










#endif
