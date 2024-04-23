#ifndef __pwmLed_h
#define __pwmLed_h

#include "gpio.h"
#include "led.h"
#include <avr/io.h>

// NOTE: Not handling pin 9 and 10 to avoid messing with my internal timer
enum pwmEnum {
  PWM_PIN3 = GPIO_PIN3,
  PWM_PIN5 = GPIO_PIN5,
  PWM_PIN6 = GPIO_PIN6,
  // PWM_PIN9 = GPIO_PIN9,
  // PWM_PIN10 = GPIO_PIN10,
  PWM_PIN11 = GPIO_PIN11,
};

#define disabledFlag (1 << 1)

class pwmLed : public led {
private:
  const uint8_t dutyCycleRegister;
  const uint8_t timerControlRegisterA;
  uint8_t timerControlRegisterASavedValue;
  constexpr uint8_t getFirstPwmRegister(pwmEnum pin);
  constexpr uint8_t getSecondPwmRegister(pwmEnum pin);
  constexpr uint8_t getDutyCycleRegister(pwmEnum pin);

public:
  pwmLed(pwmEnum pin, uint8_t dutyCycle);
  void setDutyCycle(uint8_t dutyCycle);
  uint8_t getDutyCycle(void);
  void toggleLed(void);
  void checkFrequencyToggle(timer *timerPtr);
};

#endif // !__pwmLed_h
