#include "timer.h"
#include "interruptDisabler.h"
#include <avr/interrupt.h>

timer *timerPtr = nullptr;

ISR(TIMER1_COMPA_vect) {
  timerPtr->msec++;
  if (timerPtr->msec >= 1000) {
    timerPtr->msec = 0;
    timerPtr->seconds++;
  }
}

timer::timer(void) {
#define compRegisterValue                                                      \
  15999      // 16MHz / 16000 = 1000, start counting from 0 so 15999
  TCNT1 = 0; // Reset timer, should start at 0 but anyway
  TCCR1B |= (1 << WGM12);    // CTC Mode
  TCCR1B |= (1 << CS10);     // No prescaler
  TIMSK1 |= (1 << OCIE1A);   // ENable interrupt
  OCR1A = compRegisterValue; // Set compare register
}

uint16_t timer::getMiliSec(void) {
  scopedInterruptDisabler scopedDisable;
  return this->msec;
}

uint16_t timer::getSeconds(void) {
  scopedInterruptDisabler scopedDisable;
  return this->seconds;
}
