#ifndef PWMDRIVER_H
#define PWMDRIVER_H

#include <Arduino.h>
#include "stm32f4xx_hal.h"

#define PWM_COMPILED_ENABLED

#ifdef PWM_COMPILED_ENABLED

class PwmDriver {
public:
    PwmDriver();

    // API - Names kept exactly as requested
    void begin(float frequencyHz);
    void updatePWM(float dutyPercent, bool enable);

private:
    // Internal helper functions
    uint32_t computeCCRfromInput(float inputPercent);
    uint32_t encodeDeadTime(uint32_t deadtime_ns, uint32_t tim_clk_hz);
    
    // Member variables
    TIM_HandleTypeDef _htim1;
    
    // Constants moved from macros to private consts for safety
    const float _inputThreshold = 35.0f;
    const float _pwmPeriodUs = 12.8f;
    const float _onA = 12.646f;
    const float _onB = 0.09846f;
    const uint32_t _deadtimeNs = 1000; // 1 µs
};

#endif

#endif // PWMDRIVER_H