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

void led::adcToFreqency(uint16_t adcVal) {
  scopedInterruptDisabler scopedDisable;
  if (adcVal == 0) {
    this->enableLed();
    this->toggleFrequency = 0xffff;
    // TODO: Add flags for this instead
    return;
  }
  float freq = ((float)10 / (float)1023) * adcVal;
  this->enableFrequencyToggle(static_cast<uint8_t>(freq));
}
