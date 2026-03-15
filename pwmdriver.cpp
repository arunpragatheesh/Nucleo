#include "pwmdriver.h"

#ifdef PWM_COMPILED_ENABLED

PwmDriver::PwmDriver() {
    // Constructor
}

uint32_t PwmDriver::computeCCRfromInput(float inputPercent) {
    if (inputPercent < _inputThreshold) {
        return 0;
    }

    if (inputPercent > 100.0f) inputPercent = 100.0f;

    float on_us = _onA - (_onB * inputPercent);

    if (on_us < 0.0f) on_us = 0.0f;
    if (on_us > _pwmPeriodUs) on_us = _pwmPeriodUs;

    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&_htim1);
    uint32_t ccr = (uint32_t)((on_us / _pwmPeriodUs) * (arr + 1));

    return ccr;
}

uint32_t PwmDriver::encodeDeadTime(uint32_t deadtime_ns, uint32_t tim_clk_hz) {
    float tDTS = 1.0f / tim_clk_hz;
    uint32_t ticks = (uint32_t)(deadtime_ns * 1e-9f / tDTS);

    if (ticks <= 127) return ticks;
    else if (ticks <= 254) return 0x80 | ((ticks / 2) - 64);
    else if (ticks <= 504) return 0xC0 | ((ticks / 8) - 32);
    else if (ticks <= 1008) return 0xE0 | ((ticks / 16) - 32);
    else return 0xFF;
}

void PwmDriver::begin(float Frequency) {
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // TIM1_CH1 → PA8
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // TIM1_CH1N → PB13
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    uint32_t timerClock = 180000000UL; 
    uint32_t period = (uint32_t)((timerClock / Frequency) - 1);

    _htim1.Instance = TIM1;
    _htim1.Init.Prescaler = 0;
    _htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    _htim1.Init.Period = period;
    _htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    _htim1.Init.RepetitionCounter = 0;
    _htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_PWM_Init(&_htim1);

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    HAL_TIM_PWM_ConfigChannel(&_htim1, &sConfigOC, TIM_CHANNEL_1);

    TIM_BreakDeadTimeConfigTypeDef sBDTR = {0};
    sBDTR.DeadTime = encodeDeadTime(_deadtimeNs, timerClock);
    sBDTR.OffStateRunMode = TIM_OSSR_DISABLE;
    sBDTR.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBDTR.LockLevel = TIM_LOCKLEVEL_OFF;
    sBDTR.BreakState = TIM_BREAK_DISABLE;
    sBDTR.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

    HAL_TIMEx_ConfigBreakDeadTime(&_htim1, &sBDTR);

    HAL_TIM_PWM_Start(&_htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&_htim1, TIM_CHANNEL_1);
    __HAL_TIM_MOE_ENABLE(&_htim1);

    TIM1->CCER |= TIM_CCER_CC1NE;
}

void PwmDriver::updatePWM(float duty, bool pwmEn) {
    if (duty < 0.0f) duty = 0.0f;
    if (duty > 100.0f) duty = 100.0f;

    if (!pwmEn) {
        __HAL_TIM_SET_COMPARE(&_htim1, TIM_CHANNEL_1, 0);
        return;
    }

    uint32_t ccr = computeCCRfromInput(duty);
    __HAL_TIM_SET_COMPARE(&_htim1, TIM_CHANNEL_1, ccr);
}

#endif