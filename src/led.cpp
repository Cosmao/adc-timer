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

void led::enableFrequencyToggle(timer *timerPtr, uint16_t onOffTimeInMS) {
  scopedInterruptDisabler scopedDisable;
  this->flags |= isUsingFrequencyFlag;
  this->msBetweenToggle = onOffTimeInMS;
  this->getNextToggleTime(timerPtr);
}

void led::getNextToggleTime(timer *timerPtr) {
  scopedInterruptDisabler scopedDisable;
  this->nextToggleMilliSeconds = this->msBetweenToggle + timerPtr->getMiliSec();
  if (this->nextToggleMilliSeconds == 0xffff) {
    this->nextToggleMilliSeconds = 535;
  }
}

// NOTE: I kinda hate this but it works
void led::checkFrequencyToggle(timer *timerPtr) {
  scopedInterruptDisabler scopedDisable;
  if ((this->flags & isUsingFrequencyFlag) != isUsingFrequencyFlag) {
    return;
  }
  if (timerPtr->getMiliSec() >= this->nextToggleMilliSeconds) {
    this->toggleLed();
    this->getNextToggleTime(timerPtr);
  }
}

void led::adcToFreqency(timer *timePtr, uint16_t adcVal) {
  scopedInterruptDisabler scopedDisable;
  if (adcVal == 0) {
    this->enableLed();
    this->flags &= ~isUsingFrequencyFlag;
    return;
  }
  float freq = ((float)10 / (float)1023) * adcVal;
  this->enableFrequencyToggle(timePtr, static_cast<uint8_t>(freq));
}
