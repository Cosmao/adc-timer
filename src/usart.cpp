#include "usart.h"
#include "interruptDisabler.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

usart *usartPtr = nullptr; // extern ptr for interrupt vec

usart::usart(uint16_t baud) {
  {
    scopedInterruptDisabler scopedDisable;
    baud = F_CPU / (16LL * baud) - 1; // calculate the baud rate
    UBRR0H = (uint8_t)(baud >> 8);
    UBRR0L = (uint8_t)baud;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // enable transmitter and reciever
    UCSR0B |=
        (1 << TXCIE0) | (1 << RXCIE0); // enable send and recieve interrupt
    UCSR0C = (3 << UCSZ00);            // 8-bit , 1stop bit
  }
  this->flags = standbyFlag;
}

// TODO: Move the bounds check
void usart::sendByte(void) {
  if ((UCSR0A & (1 << UDRE0)) && (this->sendIndex < (bufferSize - 1))) {
    if (this->sendBuffer[this->sendIndex] != '\0') {
      scopedInterruptDisabler scopedDisable;
      UDR0 = this->sendBuffer[this->sendIndex];
      this->sendIndex++;
    } else {
      this->flags &= ~(sendingFlag);
      this->flags |= sendBufferClearFlag;
    }
  }
}

void usart::sendString(const char *string) {
  if (this->getUsartStatus() == usartStatus::usartStandby) {
    this->flags |= sendingFlag;
    this->flags &= ~sendBufferClearFlag;
    this->sendIndex = 0;
    uint8_t counter = 0;
    while (*string != '\0' && counter < bufferSize - 3) {
      this->sendBuffer[counter] = *string;
      counter++;
      string++;
    }
    this->sendBuffer[counter] = '\0';
    this->sendByte();
  } else {
    this->flags |= datalossFlag;
  }
}

// TODO: Move the bounds check
void usart::readByte(void) {
  if (((UCSR0A & (1 << RXC0))) && (this->recieveIndex < (bufferSize - 1))) {
    scopedInterruptDisabler interruptDisabler;
    this->flags |= recievingFlag;
    this->flags &= ~standbyFlag;
    this->recieveBuffer[this->recieveIndex] = UDR0;
    if (this->recieveBuffer[this->recieveIndex] == '\0') {
      this->flags &= ~recievingFlag;
      this->flags |= standbyFlag | incomingDataReadyFlag;
      this->recieveIndex = 0;
    } else {
      this->recieveIndex++;
    }
  }
}

bool usart::incomingDataReady(void) {
  return ((this->flags & incomingDataReadyFlag) == incomingDataReadyFlag);
}

bool usart::sendBufferClear(void) {
  return ((this->flags & sendBufferClearFlag) == sendBufferClearFlag);
}

// NOTE: Interrupts disabled here to insure integrity of the data
void usart::readData(char *string) {
  scopedInterruptDisabler interruptDisabler;
  uint8_t counter = 0;
  while ((this->recieveBuffer[counter] != '\0') &&
         (counter < (bufferSize - 2))) {
    string[counter] = this->recieveBuffer[counter];
    counter++;
  }
  string[counter] = '\0';
  this->flags &= ~incomingDataReadyFlag;
}

usartStatus usart::getUsartStatus(void) {
  if ((this->flags & sendingFlag) == sendingFlag) {
    return usartStatus::usartIsSending;
  } else if ((this->flags & recievingFlag) == recievingFlag) {
    return usartStatus::usartIsRecieving;
  } else if ((this->flags & standbyFlag) == standbyFlag) {
    return usartStatus::usartStandby;
  }
  return usartStatus::usartUndefined;
}

ISR(USART_RX_vect) {
  usartStatus curStatus = usartPtr->getUsartStatus();
  if (curStatus == usartStatus::usartIsRecieving ||
      curStatus == usartStatus::usartStandby) {
    usartPtr->readByte();
  }
}

ISR(USART_TX_vect) {
  usartStatus curStatus = usartPtr->getUsartStatus();
  if (curStatus == usartStatus::usartIsSending ||
      curStatus == usartStatus::usartStandby) {
    usartPtr->sendByte();
  }
}

uint8_t decodeIncomingAmount(const char *string) {
  uint8_t value = 0;
  uint8_t strLength = 0;
  while (string[strLength] != '\0') {
    strLength++;
  }
  for (uint8_t i = 1; i < 4; i++) {
    if ((string[strLength - i] >= 48) && (string[strLength - i] <= 57)) {
      value += (string[strLength - i] - 48) * decodeMultiplier(i - 1);
    }
  }
  return value;
}

// NOTE: Kinda lazy way of doing it, should probably put the string inside
// progmem, small enough that I wont care
uint8_t decodeMessage(const char *string) {
  static const char cmpString[] = "ledpower";
  return strncmp(cmpString, string, 8);
}

uint8_t decodeMultiplier(uint8_t loopCount) {
  switch (loopCount) {
  case 0:
    return 1;
    break;
  case 1:
    return 10;
    break;
  case 2:
    return 100;
    break;
  default:
    return 1;
    break;
  }
}