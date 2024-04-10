#include "adc.h"
#include "interruptDisabler.h"
#include <avr/interrupt.h>

#define adcMaxPort 5

adc *adcPtr = nullptr;

adc::adc(void) {
  scopedInterruptDisabler interruptDisabler;
  PRR &= ~(1 << PRADC);    // Power on
  ADMUX |= ((1 << REFS0)); // Use internal ref volt
  // TODO: Change the prescaler to be in the good gap
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); // 32 prescaler division
  ADCSRA |= (1 << ADEN);                 // turn ADC on
  ADCSRA |= (1 << ADIE);                 // Enable interrupt
  this->flags &= ~isConvertingFlag;
}

void adc::beginRead(uint8_t port) {
  if (port > adcMaxPort) {
    return;
  }
  scopedInterruptDisabler interruptDisabler;
  this->flags |= isConvertingFlag;
  ADMUX &= 0xF0;         // mask the port bits out
  ADMUX |= port;         // assign port bits
  ADCSRA |= (1 << ADSC); // start conversion
}

void adc::startRead(uint8_t port) {
  if ((this->flags & isConvertingFlag) == isConvertingFlag) {
    return;
  }
  this->flags &= ~dataReadyFlag;
  this->beginRead(port);
}

bool adc::dataReady(void) {
  return ((this->flags & dataReadyFlag) == dataReadyFlag);
}

uint16_t adc::readData(void) {
  scopedInterruptDisabler interruptDisabler;
  this->flags &= ~dataReadyFlag;
  return this->value;
}

ISR(ADC_vect) {
  scopedInterruptDisabler interruptDisabler;
  uint16_t value = ADCL; // Read 8 low bits
  value += (ADCH << 8);  // read 2 high bits
  value &= 0x3FF;        // mask of the 6 high bits, only 10 bit accuracy
  adcPtr->value = value;
  adcPtr->flags |= dataReadyFlag;
  adcPtr->flags &= ~isConvertingFlag;
}
