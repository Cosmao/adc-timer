#ifndef _timer_h
#define _timer_h

#include <avr/io.h>

#define wrapAroundValue (0xffff % 1000)

extern "C" void TIMER1_COMPA_vect(void) __attribute__((signal));

class timer {
  friend void ::TIMER1_COMPA_vect(void);

private:
  volatile uint16_t msec = 0;
  volatile uint16_t seconds = 0;

public:
  timer(void);
  uint16_t getMiliSec(void);
  uint16_t getSeconds(void);
};

extern timer *timerPtr;

#endif // !_timer_h
