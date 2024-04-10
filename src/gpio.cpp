#include "gpio.h"
#include "interruptDisabler.h"

#define bankSize 8
#define maxPin 13

gpio::gpio(pinEnum pin, ioEnum io)
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

constexpr uint8_t gpio::getReadBank(pinEnum pin) {
  switch (pin) {
  case pinEnum::GPIO_PIN0... pinEnum::GPIO_PIN7:
    return (uint8_t)((uint16_t)&PIND & 0xFF);
  case pinEnum::GPIO_PIN8... pinEnum::GPIO_PIN13:
    return (uint8_t)((uint16_t)&PINB & 0xFF);
  default:
    return 0;
    break;
  }
}

constexpr uint8_t gpio::getReadOffset(pinEnum pin) {
  uint8_t offset = pin;
  if (pin >= bankSize) {
    offset -= bankSize;
  }
  return (1 << offset);
}

constexpr uint8_t gpio::getWriteBank(pinEnum pin) {
  switch (pin) {
  case pinEnum::GPIO_PIN0... pinEnum::GPIO_PIN7:
    return (uint8_t)((uint16_t)&PORTD & 0xFF);
  case pinEnum::GPIO_PIN8... pinEnum::GPIO_PIN13:
    return (uint8_t)((uint16_t)&PORTB & 0xFF);
  default:
    return 0;
    break;
  }
}

constexpr uint8_t gpio::getWriteOffset(pinEnum pin) {
  return getReadOffset(pin);
}

constexpr uint8_t gpio::getDDRBank(pinEnum pin) {
  switch (pin) {
  case pinEnum::GPIO_PIN0... pinEnum::GPIO_PIN7:
    return (uint8_t)((uint16_t)&DDRD & 0xFF);
  case pinEnum::GPIO_PIN8... pinEnum::GPIO_PIN13:
    return (uint8_t)((uint16_t)&DDRB & 0xFF);
  default:
    return 0;
    break;
  }
}

constexpr uint8_t gpio::getDDROffset(pinEnum pin) { return getReadOffset(pin); }

volatile uint8_t *getPtr(uint8_t offset) {
  volatile uint8_t *ptr = reinterpret_cast<uint8_t *>(offset);
  return ptr;
}
