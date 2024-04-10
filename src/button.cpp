#include "button.h"
#include "gpio.h"
#include "interruptDisabler.h"
#include "led.h"

ledButton::ledButton(uint8_t pin, led &ledRef)
    : gpio(pin, ioEnum::GPIO_INPUT), ledRef(ledRef) {}

bool ledButton::isButtonPressed(void) {
  volatile uint8_t *inputRegisterPtr = getPtr(this->readBank);
  scopedInterruptDisabler scopedDisable;
  if ((*inputRegisterPtr & this->readOffset) == this->readOffset) {
    return false;
  }
  return true;
}

void ledButton::toggleButtonLed(void) { this->ledRef.toggleLed(); }
