#ifndef __adc_h
#define __adc_h

#include <avr/io.h>

#define dataReadyFlag (1 << 0)
#define isConvertingFlag (1 << 1)

extern "C" void ADC_vect(void) __attribute__((signal));

class adc {
  friend void::ADC_vect(void);

private:
  uint8_t flags;
  uint16_t value;
  void beginRead(uint8_t port);

public:
  adc(void);
  void startRead(uint8_t port);
  bool dataReady(void);
  uint16_t readData(void);
};

extern adc *adcPtr;

#endif // !__adc_h
