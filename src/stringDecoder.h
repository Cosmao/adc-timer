#ifndef __stringDecoder_h
#define __stringDecoder_h

#include <avr/io.h>

enum message {
  adcToPWM,
  echoInput,
  ledFreq,
  ledDutyCycle,
  noMatch,
};

message decodeMessage(const char *str);
uint16_t getFirstNumber(const char *str);
uint16_t getSecondNumber(const char *str);

#endif // !__stringDecoder_h
