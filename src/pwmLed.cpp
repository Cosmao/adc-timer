#include "pwmLed.h"
#include "gpio.h"
#include "interruptDisabler.h"
#include "led.h"
#include <avr/io.h>

pwmLed::pwmLed(pwmEnum pin, uint8_t dutyCycle)
    : led(pinEnum(pin)), dutyCycleRegister(this->getDutyCycleRegister(pin)),
      timerControlRegisterA(this->getFirstPwmRegister(pin)) {
  // NOTE: This would have to be rewritten if it should handle timer 1
  volatile uint8_t *regPtr = getPtr(this->timerControlRegisterA);
  *regPtr = ((1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) |
             (1 << WGM00)); // Non inverting fast PWM mode
  regPtr = getPtr(this->getSecondPwmRegister(pin));
  *regPtr = (1 << CS02); // 256 prescaler
  regPtr = getPtr(this->dutyCycleRegister);
  *regPtr = dutyCycle;
}

void pwmLed::setDutyCycle(uint8_t dutyCycle) {
  volatile uint8_t *regPtr = getPtr(this->dutyCycleRegister);
  *regPtr = dutyCycle;
}

uint8_t pwmLed::getDutyCycle(void) {
  volatile uint8_t *regPtr = getPtr(this->dutyCycleRegister);
  return *regPtr;
}

void pwmLed::toggleLed(void) {
  volatile uint8_t *regPtrs = getPtr(this->timerControlRegisterA);
  if ((this->flags & disabledFlag) == disabledFlag) {
    *regPtrs = this->timerControlRegisterASavedValue;
    this->flags &= ~disabledFlag;
    if ((this->flags & delayedDutyCycleFlag) == delayedDutyCycleFlag) {
      this->setDutyCycle(this->delayedDutyCycle);
      this->flags &= ~delayedDutyCycleFlag;
    }
  } else {
    this->timerControlRegisterASavedValue = *regPtrs;
    *regPtrs &= 0x0f;
    this->flags |= disabledFlag;
  }
}

// NOTE: I kinda hate this but it works
void pwmLed::checkFrequencyToggle(timer *timerPtr) {
  scopedInterruptDisabler scopedDisable;
  if ((this->flags & isUsingFrequencyFlag) != isUsingFrequencyFlag) {
    return;
  }
  if (timerPtr->getMiliSec() >= this->nextToggleMilliSeconds) {
    this->toggleLed();
    this->getNextToggleTime(timerPtr);
  }
}

// NOTE: if you change the dutyCycle when led is off it also turns on, this
// delays the change until its turned on again
void pwmLed::delayedSetDutyCycle(uint8_t dutyCycle) {
  if ((this->flags & disabledFlag) == disabledFlag) {
    this->flags |= delayedDutyCycleFlag;
    this->delayedDutyCycle = dutyCycle;
  } else {
    this->setDutyCycle(dutyCycle);
    ;
  }
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
