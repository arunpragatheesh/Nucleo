#ifndef ADCDRIVER_H
#define ADCDRIVER_H

#include <Arduino.h>

class AdcDriver {
  public:
    AdcDriver();

    // Your exact API names
    void begin();
    float filterInput(int Input);

  private:
    float _prevADCValue;
    bool _first;
};

#endif