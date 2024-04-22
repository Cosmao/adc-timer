#include "pwmLed.h"
#include "gpio.h"
#include "led.h"
#include <avr/io.h>

pwmLed::pwmLed(pwmEnum pin)
    : led(pinEnum(pin)), dutyCycleRegister(this->getDutyCycleRegister(pin)) {
  volatile uint8_t *regPtr = getPtr(this->getFirstPwmRegister(pin));
  *regPtr = ((1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00));
  regPtr = getPtr(this->getSecondPwmRegister(pin));
  *regPtr = (1 << CS02);
}

void pwmLed::setDutyCycle(uint8_t dutyCycle) {
  this->pwmDutyCycle = dutyCycle;
  volatile uint8_t *regPtr = getPtr(this->dutyCycleRegister);
  *regPtr = this->pwmDutyCycle;
}

constexpr uint8_t pwmLed::getFirstPwmRegister(pwmEnum pin) {
  switch (pin) {
  case pwmEnum::PWM_PIN3:
  case pwmEnum::PWM_PIN11:
    return (uint8_t)((uint16_t)&TCCR2A & 0xFF);
  case pwmEnum::PWM_PIN5:
  case pwmEnum::PWM_PIN6:
    return (uint8_t)((uint16_t)&TCCR0A & 0xFF);
  // case pwmEnum::PWM_PIN9:
  // case pwmEnum::PWM_PIN10:
  //   return (uint8_t)((uint16_t)&TCCR1A & 0xFF);
  default:
    return 0;
  }
}

constexpr uint8_t pwmLed::getSecondPwmRegister(pwmEnum pin) {
  switch (pin) {
  case pwmEnum::PWM_PIN3:
  case pwmEnum::PWM_PIN11:
    return (uint8_t)((uint16_t)&TCCR2B & 0xFF);
  case pwmEnum::PWM_PIN5:
  case pwmEnum::PWM_PIN6:
    return (uint8_t)((uint16_t)&TCCR0B & 0xFF);
  // case pwmEnum::PWM_PIN9:
  // case pwmEnum::PWM_PIN10:
  //   return (uint8_t)((uint16_t)&TCCR1B & 0xFF);
  default:
    return 0;
  }
}

constexpr uint8_t pwmLed::getDutyCycleRegister(pwmEnum pin) {
  switch (pin) {
  case pwmEnum::PWM_PIN3:
    return (uint8_t)((uint16_t)&OCR2B & 0xFF);
  case pwmEnum::PWM_PIN5:
    return (uint8_t)((uint16_t)&OCR0B & 0xFF);
  case pwmEnum::PWM_PIN6:
    return (uint8_t)((uint16_t)&OCR0A & 0xFF);
  case pwmEnum::PWM_PIN11:
    return (uint8_t)((uint16_t)&OCR2A & 0xFF);
  // case pwmEnum::PWM_PIN9:
  //   return (uint8_t)((uint16_t)&OCR1A & 0xFF);
  // case pwmEnum::PWM_PIN10:
  //   return (uint8_t)((uint16_t)&OCR1B & 0xFF);
  default:
    return 0;
  }
}
