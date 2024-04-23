#ifndef __led_h
#define __led_h

#include "gpio.h"
#include "timer.h"
#include <avr/io.h>

#define isUsingFrequencyFlag (1 << 0)

class led : public gpio {
protected:
  uint16_t msBetweenToggle = 0;
  uint16_t nextToggleMilliSeconds = 0;
  void getNextToggleTime(timer *timerPtr);

public:
  led(pinEnum pin);
  void toggleLed(void);
  void enableLed(void);
  void disableLed(void);
  void changeFrequencyToggle(timer *timerPtr, uint16_t onOffTimeInMS);
  void disableFrequencyToggle(void);
  void checkFrequencyToggle(timer *timerPtr);
  void adcToFreqency(timer *timePtr, uint16_t adcVal);
};

#endif // !__led_h
