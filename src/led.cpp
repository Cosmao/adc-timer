#include "led.h"
#include "gpio.h"
#include "interruptDisabler.h"

led::led(pinEnum pin) : gpio(pin, ioEnum::GPIO_OUTPUT) {}

void led::toggleLed(void) {
  volatile uint8_t *outputRegisterPtr = getPtr(this->writeBank);
  scopedInterruptDisabler scopedDisable;
  *outputRegisterPtr ^= this->writeOffset;
}

void led::enableLed(void) {
  volatile uint8_t *outputRegisterPtr = getPtr(this->writeBank);
  scopedInterruptDisabler scopedDisable;
  *outputRegisterPtr |= this->writeOffset;
}

void led::disableLed(void) {
  volatile uint8_t *outputRegisterPtr = getPtr(this->writeBank);
  scopedInterruptDisabler scopedDisable;
  *outputRegisterPtr &= ~this->writeOffset;
}

void led::enableFrequencyToggle(uint8_t toggleHz) {
  this->toggleFrequency = 1000 / toggleHz;
}

void led::checkFrequencyToggle(uint16_t milliSeconds) {
  scopedInterruptDisabler scopedDisable;
  if ((milliSeconds % this->toggleFrequency == 0) &&
      (milliSeconds != this->lastToggle)) {
    this->toggleLed();
    this->lastToggle = milliSeconds;
  }
}
