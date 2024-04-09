#include "interruptDisabler.h"
#include <avr/io.h>

scopedInterruptDisabler::scopedInterruptDisabler(void){
  this->registerState = SREG; 
  SREG &= ~(1 << SREG_I);
}

scopedInterruptDisabler::~scopedInterruptDisabler(void){
  SREG = this->registerState;
}
