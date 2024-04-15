#ifndef __led_h
#define __led_h

#include "gpio.h"
#include <avr/io.h>

class led : public gpio {
private:
  uint16_t toggleFrequency = 0;
  uint16_t lastToggle = 0;

public:
  led(pinEnum pin);
  void toggleLed(void);
  void enableLed(void);
  void disableLed(void);
  void enableFrequencyToggle(uint8_t toggleHz);
  void checkFrequencyToggle(uint16_t milliSeconds);
  void adcToFreqency(uint16_t adcVal);
};

#endif // !__led_h
