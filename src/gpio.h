#ifndef __gpio_h
#define __gpio_h

#include <avr/io.h>

enum ioEnum { GPIO_INPUT = 0, GPIO_OUTPUT = 1 };

class gpio {
protected:
  uint8_t flags;
  const uint8_t pin;
  const uint16_t readBank;
  const uint8_t readOffset;
  const uint16_t writeBank;
  const uint8_t writeOffset;
  constexpr uint16_t getReadBank(uint8_t pin);
  constexpr uint8_t getReadOffset(uint8_t pin);
  constexpr uint16_t getWriteBank(uint8_t pin);
  constexpr uint8_t getWriteOffset(uint8_t pin);
  constexpr uint16_t getDDRBank(uint8_t pin);
  constexpr uint8_t getDDROffset(uint8_t pin);

public:
  explicit gpio(uint8_t pin, enum ioEnum);
};

volatile uint8_t *getPtr(uint8_t offset);

#endif // !__gpio_h
