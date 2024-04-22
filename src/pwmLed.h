#ifndef __pwmLed_h
#define __pwmLed_h

#include "gpio.h"
#include "led.h"
#include <avr/io.h>

// TODO: Get all the timers
// 3, 5, 6, 9, 10, 11
// 3 = OC2B
// 5 = OC0B
// 6 = OC0A
// 9 = OC1A
// 10 = OC1B
// 11 = OC2A

enum pwmEnum {
  PWM_PIN3 = GPIO_PIN3,
  PWM_PIN5 = GPIO_PIN5,
  PWM_PIN6 = GPIO_PIN6,
  // PWM_PIN9 = GPIO_PIN9,
  // PWM_PIN10 = GPIO_PIN10,
  PWM_PIN11 = GPIO_PIN11,
};

class pwmLed : private led {
private:
  uint8_t pwmDutyCycle = 0;
  const uint8_t dutyCycleRegister;
  constexpr uint8_t getFirstPwmRegister(pwmEnum pin);
  constexpr uint8_t getSecondPwmRegister(pwmEnum pin);
  constexpr uint8_t getDutyCycleRegister(pwmEnum pin);

public:
  pwmLed(pwmEnum pin);
  void setDutyCycle(uint8_t dutyCycle);
};

#endif // !__pwmLed_h
