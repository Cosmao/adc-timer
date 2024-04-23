#include "stringDecoder.h"
#include <string.h>

message decodeMessage(const char *string) {
  const char *strings[] = {"adcpwm", "echo", "ledpowerfreq", "ledduty"};
  uint8_t index = 0;
  for (auto s : strings) {
    if (strncmp(string, s, strlen(s)) == 0) {
      return message(index);
    }
    index++;
  }
  return message::noMatch;
}
