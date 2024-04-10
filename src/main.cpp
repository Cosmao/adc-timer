#include "button.h"
#include "led.h"
#include "usart.h"
#include <avr/io.h>
#include <util/delay.h>

#define baudRate 9600

int main(void) {
  led led(8);
  ledButton button(9, led);
  usart usart(baudRate);
  // WARNING: enable global interrupts after setting this ptr! Can crash
  // otherwise
  usartPtr = &usart;
  SREG |= (1 << SREG_I); // enable interrupts
  while (true) {
    usart.checkData();
    if (button.isButtonPressed()) {
      button.toggleButtonLed();
      // NOTE: delay is only here to make it usable, havent set up a proper
      // debounce with timers
      _delay_ms(250);
    }
    if (usart.incomingDataReady()) {
      char charbuff[bufferSize];
      usart.readData(charbuff);
      // NOTE: Just echoing back whatever we get
      usart.sendString(charbuff);
      if (decodeMessage(charbuff) == 0) {
        uint8_t val = decodeIncomingAmount(charbuff);
        if (val > 127) {
          led.enableLed();
        } else {
          led.disableLed();
        }
      }
    }
  }
}
