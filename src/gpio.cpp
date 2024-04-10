#include "gpio.h"
#include "interruptDisabler.h"

#define bankSize 8
#define maxPin 13

gpio::gpio(uint8_t pin, ioEnum io)
    : pin(pin), readBank(getReadBank(pin)), readOffset(getReadOffset(pin)),
      writeBank(getWriteBank(pin)), writeOffset(getWriteOffset(pin)) {
  volatile uint8_t *dataDirectionPtr = getPtr(getDDRBank(pin));
  scopedInterruptDisabler scopedDisable;
  switch (io) {
  case ioEnum::GPIO_INPUT:
    *dataDirectionPtr &= ~this->writeOffset;
    break;
  case ioEnum::GPIO_OUTPUT:
    *dataDirectionPtr |= this->writeOffset;
    break;
  }
}

constexpr uint8_t gpio::getReadBank(uint8_t pin) {
  if (pin < bankSize) {
    // FIXME: Find a way to save these as uint8_t and save some memory
    // Can just make it pointers instead and save on execution time
    // and implementation time
    return (uint8_t)((uint16_t)&PIND & 0xFF);
  } else if (pin <= maxPin) {
    return (uint8_t)((uint16_t)&PINB & 0xFF);
  }
  // NOTE: I kinda want to throw a error at compile time here since its out of
  // bounds
  return 0;
}

constexpr uint8_t gpio::getReadOffset(uint8_t pin) {
  if (pin >= bankSize) {
    pin -= bankSize;
  }
  return (1 << pin);
}

constexpr uint8_t gpio::getWriteBank(uint8_t pin) {
  if (pin < bankSize) {
    return (uint8_t)((uint16_t)&PORTD & 0xFF);
  } else if (pin <= maxPin) {
    return (uint8_t)((uint16_t)&PORTB & 0xFF);
  }
  // NOTE: I kinda want to throw a error at compile time here since its out of
  // bounds
  return 0;
}

constexpr uint8_t gpio::getWriteOffset(uint8_t pin) {
  return getReadOffset(pin);
}

constexpr uint8_t gpio::getDDRBank(uint8_t pin) {
  if (pin < bankSize) {
    return (uint8_t)((uint16_t)&DDRD & 0xFF);
    return DDRD;
  } else if (pin <= maxPin) {
    return (uint8_t)((uint16_t)&DDRB & 0xFF);
  }
  // NOTE: I kinda want to throw a error at compile time here since its out of
  // bounds
  return 0;
}

constexpr uint8_t gpio::getDDROffset(uint8_t pin) { return getReadOffset(pin); }

volatile uint8_t *getPtr(uint8_t offset) {
  volatile uint8_t *ptr = reinterpret_cast<uint8_t *>(offset);
  return ptr;
}
