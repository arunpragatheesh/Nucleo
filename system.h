#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>

class System {
  public:
    System();
    void begin();

    void runHeartbeat();   // The blink logic

    // These return true once every interval
    bool is5ms();
    bool is20ms();
    bool is50ms();

  private:
    unsigned long _last5ms;
    unsigned long _last20ms;
    unsigned long _last50ms;

    // Heartbeat variables
    unsigned long _lastToggle;
    bool _ledState;
    const uint32_t _blinkPeriod = 1000;
};

#endif