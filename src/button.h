#ifndef __button_h
#define __button_h

#include "gpio.h"
#include "led.h"
#include <avr/io.h>

class ledButton : public gpio{
  friend gpio;

private:
  led &ledRef;
  volatile uint8_t disableUntil;

public:
  ledButton(uint8_t pin, led &ledRef);
  bool isButtonPressed(void);
  void toggleButtonLed(void);
};

#endif // !__button_h
