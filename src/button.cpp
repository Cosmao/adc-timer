#include "button.h"
#include "gpio.h"
#include "interruptDisabler.h"
#include "led.h"

ledButton::ledButton(uint8_t pin, led &ledRef)
    : gpio(pin, ioEnum::GPIO_INPUT), ledRef(ledRef) {}

bool ledButton::isButtonPressed(void) {
  volatile uint8_t *ptr = getPtr(this->readBank);
  scopedInterruptDisabler scopedDisable;
  if ((*ptr & this->readOffset) == this->readOffset) {
    return false;
  }
  return true;
}

void ledButton::toggleButtonLed(void) { this->ledRef.toggleLed(); }
