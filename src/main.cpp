#include "adc.h"
#include "button.h"
#include "gpio.h"
#include "led.h"
#include "usart.h"
#include <avr/io.h>
// #include <stdio.h>
#include <util/delay.h>

#define baudRate 9600
#define ledPin pinEnum::GPIO_PIN3
#define buttonPin pinEnum::GPIO_PIN4
#define adcPin 0

int main(void) {
  led led(ledPin);
  ledButton button(buttonPin, led);
  adc adc;
  usart usart(baudRate);
  // WARNING: enable global interrupts only after setting pointers! Can crash
  // otherwise
  usartPtr = &usart;
  adcPtr = &adc;
  SREG |= (1 << SREG_I); // enable interrupts
  usart.sendString("Starting\n\r");
  char charbuff[bufferSize];
  while (true) {
    usart.handleData();
    adc.startRead(adcPin);

    if (button.isButtonPressed()) {
      button.toggleButtonLed();
      // NOTE: delay is only here to make it usable, havent set up a proper
      // debounce with timers
      _delay_ms(250);
    }

    if (usart.incomingDataReady()) {
      usart.readData(charbuff);
      // NOTE: Just echoing back whatever we get
      usart.sendString(charbuff);

      // if (decodeMessage(charbuff) == 0) {
      //   uint8_t val = decodeIncomingAmount(charbuff);
      //   if (val > 127) {
      //     led.enableLed();
      //   } else {
      //     led.disableLed();
      //   }
      // }
    }

    // if (adc.dataReady()) {
    // char strBuff[bufferSize];
    // sprintf(strBuff, "ADC: %u\n\r", adc.readData());
    // usart.sendString(strBuff);
    // }
  }
}
