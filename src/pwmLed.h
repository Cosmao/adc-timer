#ifndef __pwmLed_h
#define __pwmLed_h

#include "gpio.h"
#include "led.h"
#include <avr/io.h>

//NOTE: Not handling pin 9 and 10 to avoid messing with my internal timer
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
