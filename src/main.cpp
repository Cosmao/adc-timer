#include "adc.h"
#include "button.h"
#include "gpio.h"
#include "led.h"
#include "pwmLed.h"
#include "timer.h"
#include "usart.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

// TODO: Make it a state machine here in main to do shit, enter states via
// strings over usart
//

#define baudRate 9600
#define ledPin pinEnum::GPIO_PIN3
#define buttonPin pinEnum::GPIO_PIN4
#define pwmLedPin pwmEnum::PWM_PIN6
#define adcPin 0

int main(void) {
  led led(ledPin);
  pwmLed pwmLed(pwmLedPin);
  ledButton button(buttonPin, led);
  adc adc;
  usart usart(baudRate);
  timer time;
  // WARNING: enable global interrupts only after setting pointers! Can crash
  // otherwise
  usartPtr = &usart;
  adcPtr = &adc;
  timerPtr = &time;
  // uint16_t lastMillis = 0;
  uint16_t seconds = 0;
  led.enableFrequencyToggle(2);
  pwmLed.setDutyCycle(255);
  SREG |= (1 << SREG_I); // enable interrupts
  usart.sendString("Starting\n\r");
  char charBuff[bufferSize];
  while (true) {
    usart.handleData();

    led.checkFrequencyToggle(time.getMiliSec());

    if (seconds != time.getSeconds()) {
      adc.startRead(adcPin);
      seconds = time.getSeconds();
    }

    if (button.isButtonPressed()) {
      button.toggleButtonLed();
      // NOTE: delay is only here to make it usable, havent set up a proper
      // debounce with timers
      _delay_ms(250);
    }

    if (usart.incomingDataReady()) {
      usart.readData(charBuff, bufferSize);
      // NOTE: Just echoing back whatever we get
      usart.sendString(charBuff);

      // if (decodeMessage(charbuff) == 0) {
      //   uint8_t val = decodeIncomingAmount(charbuff);
      //   if (val > 127) {
      //     led.enableLed();
      //   } else {
      //     led.disableLed();
      //   }
      // }
    }

    if (adc.dataReady()) {
      char strBuff[bufferSize];
      uint16_t adcVal = adc.readData();
      sprintf(strBuff, "ADC: %u\n\r", adcVal);
      usart.sendString(strBuff);
      // led.enableFrequencyToggle(102300 / adcVal);
      led.adcToFreqency(adcVal);
    }
  }
}
