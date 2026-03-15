#include "system.h"

System::System() {
    unsigned long start = millis();
    _last5ms = start;
    _last20ms = start;
    _last50ms = start;

    _ledState = false;
}

void System::begin() {
    pinMode(LED_BUILTIN, OUTPUT); // PA5
}

bool System::is5ms() {
    if (millis() - _last5ms >= 5) {
        _last5ms += 5; // Drift-free increment
        return true;
    }
    return false;
}

bool System::is20ms() {
    if (millis() - _last20ms >= 20) {
        _last20ms += 20;
        return true;
    }
    return false;
}

bool System::is50ms() {
    if (millis() - _last50ms >= 50) {
        _last50ms += 50;
        return true;
    }
    return false;
}

void System::runHeartbeat() {
    unsigned long now = millis();
    if (now - _lastToggle >= _blinkPeriod) {
        _lastToggle = now; // Or _lastToggle += _blinkPeriod for perfect phase
        _ledState = !_ledState;
        digitalWrite(LED_BUILTIN, _ledState);
    }
}