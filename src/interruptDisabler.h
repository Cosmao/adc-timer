#ifndef __interrupt_disabler_h
#define __interrupt_disabler_h

#include <avr/io.h>

class scopedInterruptDisabler {
private:
  uint8_t registerState;

public:
  scopedInterruptDisabler(void);
  ~scopedInterruptDisabler(void);
};

#endif // !__interrupt_disabler_h
