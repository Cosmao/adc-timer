#ifndef __gpio_h
#define __gpio_h

#include <avr/io.h>

enum ioEnum { GPIO_INPUT = 0, GPIO_OUTPUT = 1 };

enum pinEnum {
  GPIO_PIN0 = 0,
  GPIO_PIN1 = 1,
  GPIO_PIN2 = 2,
  GPIO_PIN3 = 3,
  GPIO_PIN4 = 4,
  GPIO_PIN5 = 5,
  GPIO_PIN6 = 6,
  GPIO_PIN7 = 7,
  GPIO_PIN8 = 8,
  GPIO_PIN9 = 9,
  GPIO_PIN10 = 10,
  GPIO_PIN11 = 11,
  GPIO_PIN12 = 12,
  GPIO_PIN13 = 13,
};

class gpio {
protected:
  uint8_t flags;
  const uint8_t pin;
  const uint8_t readBank;
  const uint8_t readOffset;
  const uint8_t writeBank;
  const uint8_t writeOffset;
  constexpr uint8_t getReadBank(enum pinEnum);
  constexpr uint8_t getReadOffset(enum pinEnum);
  constexpr uint8_t getWriteBank(enum pinEnum);
  constexpr uint8_t getWriteOffset(enum pinEnum);
  constexpr uint8_t getDDRBank(enum pinEnum);
  constexpr uint8_t getDDROffset(enum pinEnum);

public:
  explicit gpio(enum pinEnum, enum ioEnum);
};

volatile uint8_t *getPtr(uint8_t offset);

#endif // !__gpio_h
