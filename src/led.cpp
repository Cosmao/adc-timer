#include "led.h"
#include "gpio.h"
#include "interruptDisabler.h"
#include "timer.h"

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

void led::changeFrequencyToggle(timer *timerPtr, uint16_t onOffTimeInMS) {
  scopedInterruptDisabler scopedDisable;
  if (onOffTimeInMS == 0) {
    this->disableFrequencyToggle();
    return;
  }
  this->flags |= isUsingFrequencyFlag;
  this->msBetweenToggle = onOffTimeInMS;
  this->getNextToggleTime(timerPtr);
}

void led::disableFrequencyToggle(void) {
  this->flags &= ~isUsingFrequencyFlag;
  this->msBetweenToggle = 0;
  this->nextToggleMilliSeconds = 0;
}

void led::getNextToggleTime(timer *timerPtr) {
  scopedInterruptDisabler scopedDisable;
  this->nextToggleMilliSeconds = this->msBetweenToggle + timerPtr->getMiliSec();
  // NOTE: This is the same as the overflow logic in the timer, just to handle
  // that edgecase and we dont get stuck in a ms value that will never be
  // reached
  if (this->nextToggleMilliSeconds == 0xffff) {
    this->nextToggleMilliSeconds = wrapAroundValue;
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
  // TODO: Update this for the new enableFreqTOggle
  float freq = ((float)10 / (float)1023) * adcVal;
  this->changeFrequencyToggle(timePtr, static_cast<uint8_t>(freq));
}
