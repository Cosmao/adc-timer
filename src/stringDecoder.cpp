#include "stringDecoder.h"
#include <stdlib.h>
#include <string.h>

#define numberBufferSize 16

messageEnum decodeMessage(const char *str) {
  const char *strings[] = {"adcpwm", "echo", "ledpowerfreq", "ledduty", "disablefreq"};
  uint8_t index = 0;
  for (auto string : strings) {
    if (strncmp(str, string, strlen(string)) == 0) {
      return messageEnum(index);
    }
    index++;
  }
  return messageEnum::noMatch;
}

stringValues getOneNumber(const char *str) {
  stringValues value;
  value.valueOne = getNumberFromString(str);
  return value;
}

stringValues getTwoNumbers(const char *str) {
  stringValues value;
  value.valueOne = getNumberFromString(str);
  while (*str != '\0') {
    if (*str == ' ') {
      str++;
      break;
    }
    str++;
  }
  value.valueTwo = getNumberFromString(str);
  return value;
}

// WARNING: I dont like using atoi() but stoi() isnt supported in avr
uint16_t getNumberFromString(const char *str) {
  while (*str != '\0') {
    if (*str == ' ') {
      str++;
      break;
    }
    str++;
  }
  return (uint16_t)atoi(str);
}
