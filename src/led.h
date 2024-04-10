#ifndef __led_h
#define __led_h

#include "gpio.h"
#include <avr/io.h>

class led : public gpio {
  friend gpio;

private:
public:
  led(pinEnum pin);
  void toggleLed(void);
  void enableLed(void);
  void disableLed(void);
};

#endif // !__led_h
