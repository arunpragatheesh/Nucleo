#include "AdcDriver.h"

AdcDriver::AdcDriver() {
    _prevADCValue = 0.0f;
    _first = true;
}

void AdcDriver::begin() {
    // Specifically for STM32/NUCLEO
    analogReadResolution(12);
    pinMode(A0, INPUT_ANALOG);
}

float AdcDriver::filterInput(int Input) {
    float ADCValue = (float)Input;

    // Initialize filter with first sample
    if (_first) {
        _prevADCValue = ADCValue;
        _first = false;
        return _prevADCValue;
    }

    // 9/10 exponential moving average filter
    _prevADCValue = (_prevADCValue * 9.0f + ADCValue) / 10.0f;

    return _prevADCValue;
}