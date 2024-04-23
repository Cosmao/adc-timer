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

typedef struct {
  uint16_t valueOne;
  uint16_t valueTwo;
} stringValues;

message decodeMessage(const char *str);
stringValues getOneNumber(const char *str);
stringValues getTwoNumbers(const char *str);
uint16_t getNumberFromString(const char *str);

#endif // !__stringDecoder_h
