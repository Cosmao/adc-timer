#include "gpio.h"
#include "interruptDisabler.h"

#define bankMaxLength 8
#define maxPin 13

gpio::gpio(uint8_t pin, ioEnum io)
    : pin(pin), readBank(getReadBank(pin)), readOffset(getReadOffset(pin)),
      writeBank(getWriteBank(pin)), writeOffset(getWriteOffset(pin)) {
  volatile uint8_t *ptr = getPtr(getDDRBank(pin));
  scopedInterruptDisabler scopedDisable;
  switch (io) {
  case ioEnum::GPIO_INPUT:
    *ptr &= ~this->writeOffset;
    break;
  case ioEnum::GPIO_OUTPUT:
    *ptr |= this->writeOffset;
    break;
  }
}

constexpr uint16_t gpio::getReadBank(uint8_t pin) {
  if (pin < bankMaxLength) {
    // FIXME: Find a way to save these as uint8_t and save some memory
    // Can just make it pointers instead and save on execution time
    // and implementation time
    return (uint16_t)&PIND;
  } else if (pin <= maxPin) {
    return (uint16_t)&PINB;
  }
  // NOTE: I kinda want to throw a error at compile time here since its out of
  // bounds
  return 0;
}

constexpr uint8_t gpio::getReadOffset(uint8_t pin) {
  if (pin >= bankMaxLength) {
    pin -= bankMaxLength;
  }
  return (1 << pin);
}

constexpr uint16_t gpio::getWriteBank(uint8_t pin) {
  if (pin < bankMaxLength) {
    return (uint16_t)&PORTD;
  } else if (pin <= maxPin) {
    return (uint16_t)&PORTB;
  }
  // NOTE: I kinda want to throw a error at compile time here since its out of
  // bounds
  return 0;
}

constexpr uint8_t gpio::getWriteOffset(uint8_t pin) {
  return getReadOffset(pin);
}

volatile uint8_t *getPtr(uint8_t offset) {
  volatile uint8_t *testPtr = reinterpret_cast<uint8_t *>(offset);
  return testPtr;
}

constexpr uint16_t gpio::getDDRBank(uint8_t pin) {
  if (pin < bankMaxLength) {
    return (uint16_t)&DDRD;
    return DDRD;
  } else if (pin <= maxPin) {
    return (uint16_t)&DDRB;
  }
  // NOTE: I kinda want to throw a error at compile time here since its out of
  // bounds
  return 0;
}

constexpr uint8_t gpio::getDDROffset(uint8_t pin) { return getReadOffset(pin); }
