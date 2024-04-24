#include "adc.h"
#include "button.h"
#include "gpio.h"
#include "pwmLed.h"
#include "stringDecoder.h"
#include "timer.h"
#include "usart.h"
#include <avr/io.h>
#include <stdio.h>

#define baudRate 9600
#define ledPin pinEnum::GPIO_PIN3
#define pwmLedPin pwmEnum::PWM_PIN3
#define buttonPin pinEnum::GPIO_PIN4
#define adcPin 0
#define shittyDebounceTime 250

#define isEchoingInput (1 << 0)
#define adcFlag (1 << 1)

int main(void) {
  pwmLed led(pwmLedPin, 25);
  ledButton button(buttonPin, led);
  adc adc;
  usart usart(baudRate);
  timer time;
  // WARNING: enable global interrupts only after setting pointers! Can crash
  // otherwise
  usartPtr = &usart;
  adcPtr = &adc;
  timerPtr = &time;
  uint16_t lastMillis = 0; // Used for simple debounce
  uint16_t seconds = 0;
  uint8_t flags = 0;
  SREG |= (1 << SREG_I); // enable interrupts
  // led.enableFrequencyToggle(timerPtr, 5000);
  usart.sendString("Starting\n\r");
  char charBuff[bufferSize];
  while (true) {

    led.checkFrequencyToggle(timerPtr);

    if (seconds != time.getSeconds() && ((flags & adcFlag) == adcFlag)) {
      adc.startRead(adcPin);
      seconds = time.getSeconds();
    }

    if (button.isButtonPressed() &&
        lastMillis + shittyDebounceTime < time.getMiliSec()) {
      led.toggleLed(); // TODO: Make this callable from the button instead
      lastMillis = time.getMiliSec(); // TODO: Move all of this into the button
    }

    if (usart.incomingDataReady()) {
      usart.readData(charBuff, bufferSize);
      if ((flags & isEchoingInput) == isEchoingInput) {
        usart.sendString(charBuff);
      }
      stringValues values;
      char buff[bufferSize];
      messageEnum decodedValue = decodeMessage(charBuff);
      switch (decodedValue) {
      case messageEnum::adcToPWM:
        flags ^= adcFlag;
        usart.sendString("Toggled adc to PWM\n\r");
        break;

      case messageEnum::echoInput:
        flags ^= isEchoingInput;
        usart.sendString("Toggled echoing\n\r");
        break;

      case messageEnum::ledDutyCycle:
        led.delayedSetDutyCycle(getOneNumber(charBuff).valueOne);
        usart.sendString("DutyCycle changed\n\r");
        break;

      case messageEnum::ledFreq:
        values = getTwoNumbers(charBuff);
        led.delayedSetDutyCycle(values.valueOne);
        led.changeFrequencyToggle(timerPtr, values.valueTwo);
        snprintf(buff, bufferSize, "PWM: %u MSWait: %u\n\r", values.valueOne,
                 values.valueTwo);
        usart.sendString(buff);
        break;

      case messageEnum::disableFreq:
        led.disableFrequencyToggle();
        usart.sendString("Disabled freq\n\r");

      case messageEnum::noMatch:
        usart.sendString("Invalid input\n\r");
        break;
      }
    }

    if (adc.dataReady()) {
      char strBuff[bufferSize];
      uint16_t adcVal = adc.readData();
      snprintf(strBuff, bufferSize, "ADC: %u\n\r", adcVal);
      usart.sendString(strBuff);
      led.adcToFreqency(timerPtr, adcVal);
    }
  }
}
