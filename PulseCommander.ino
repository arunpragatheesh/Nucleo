/*!
 * @file Capsealer.ino
 * @brief ....
 * @version  V1.0
 * @date  2025-12-15
 * @url ***
 */

#include <Wire.h>
#include "mylcdlib.h"
#include "system.h"
#include "adcdriver.h"

// Create instances for libraries
System systemTask;
MyLcdLib mylcd(0x2D, 16, 2);
AdcDriver adc; 

// ---------- Compile-time options ----------
#define PWM_COMPILED_ENABLED

#ifdef PWM_COMPILED_ENABLED
  #include <pwmdriver.h>
  PwmDriver pwm;

  float Frequency  = 78125.0f;   // Hz
  float DutyCycle  = 0.0f;       // %
#endif


//----------------------------------------------------------------------------------------
float allParams[40];


void setup() {

  Serial.begin(115200);

  systemTask.begin();

  mylcd.begin();
  mylcd.assignDataBuffer(allParams);

  adc.begin();

  #ifdef PWM_COMPILED_ENABLED
    pwm.begin(Frequency);
  #endif

}

void loop() {

  static float FilteredValue=0;

  // ---- 5 ms tasks ----
  if (systemTask.is5ms()){

  }

  // ---- 20 ms tasks ----
  if (systemTask.is20ms()){
    
    FilteredValue = adc.filterInput(analogRead(A0));

    
    #ifdef PWM_COMPILED_ENABLED
      // Map ADC to duty cycle (0–100%)
      DutyCycle = (FilteredValue * 100.0f) / 4095.0f;

      // Update PWM duty cycle only
      pwm.updatePWM(DutyCycle, true);
    #endif
       
  }

  // ---- 50 ms tasks ----
  if (systemTask.is50ms()){

    allParams[IDX_OP_FREQ] = (float)Frequency/1000.0f;
    allParams[IDX_DUTY] = DutyCycle;
    allParams[IDX_A] = 2;
    allParams[IDX_B] = 3;
    allParams[IDX_C] = 4;
    allParams[IDX_D] = 5;
    allParams[IDX_E] = 6;
    allParams[IDX_F] = 7;

    mylcd.manageUI();    
    
  }

  systemTask.runHeartbeat();

}
