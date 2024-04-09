#include "led.h"
#include "gpio.h"
#include "interruptDisabler.h"

led::led(uint8_t pin) : gpio(pin, ioEnum::GPIO_OUTPUT) {}

void led::toggleLed(void){
  volatile uint8_t *ptr = getPtr(this->writeBank);
  scopedInterruptDisabler scopedDisable;
  *ptr ^= this->writeOffset;
}

void led::enableLed(void){
  volatile uint8_t *ptr = getPtr(this->writeBank);
  scopedInterruptDisabler scopedDisable;
  *ptr |= this->writeOffset;
}

void led::disableLed(void){
  volatile uint8_t *ptr = getPtr(this->writeBank);
  scopedInterruptDisabler scopedDisable;
  *ptr &= ~this->writeOffset;
}