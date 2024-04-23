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
        (1 << TXCIE0) | (1 << RXCIE0); // enable send and transmit interrupt
    UCSR0C = (3 << UCSZ00);            // 8-bit , 1stop bit
    this->flags |= sendBufferClearFlag;
  }
}

bool usart::isInRange(uint8_t index) {
  if (index < (bufferSize - 1)) {
    return true;
  }
  return false;
}

bool usart::dataGood(uint8_t *buff, uint8_t index) {
  if (buff[index] != '\0') {
    return true;
  }
  return false;
}

void usart::sendByte(void) {
  if ((UCSR0A & (1 << UDRE0)) == (1 << UDRE0)) {
    scopedInterruptDisabler interruptDisabler;
    this->flags &= ~actOutgoingDataFlag;
    if (this->isInRange(this->sendIndex)) {
      if (this->dataGood(this->sendBuffer, this->sendIndex)) {
        UDR0 = this->sendBuffer[this->sendIndex];
        this->sendIndex++;
      } else {
        this->flags |= sendBufferClearFlag;
      }
    }
  }
}

void usart::sendString(const char *string) {
  if (this->sendBufferClear()) {
    this->flags &= ~sendBufferClearFlag;
    this->sendIndex = 0;
    uint8_t counter = 0;
    while (*string != '\0' && counter < bufferSize - 2) {
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

void usart::readByte(void) {
  if ((UCSR0A & (1 << RXC0)) == (1 << RXC0)) {
    scopedInterruptDisabler interruptDisabler;
    this->flags &= ~actIncomingDataFlag;
    if (this->isInRange(this->recieveIndex)) {
      this->recieveBuffer[this->recieveIndex] = UDR0;
      if (this->recieveBuffer[this->recieveIndex] == '\0') {
        this->flags |= incomingDataReadyFlag;
        this->recieveIndex = 0;
      } else {
        this->recieveIndex++;
      }
    } else {
      this->recieveBuffer[bufferSize - 1] = '\0';
      this->flags |= incomingDataReadyFlag;
      this->recieveIndex = 0;
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
void usart::readData(char *string, uint8_t incomingBufferSize) {
  scopedInterruptDisabler interruptDisabler;
  uint8_t counter = 0;
  while ((this->recieveBuffer[counter] != '\0') &&
         (counter < (bufferSize - 2)) && (counter < (incomingBufferSize - 2))) {
    string[counter] = this->recieveBuffer[counter];
    counter++;
  }
  string[counter] = '\0';
  this->flags &= ~incomingDataReadyFlag;
}

void usart::handleData(void) {
  if ((this->flags & actIncomingDataFlag) == actIncomingDataFlag) {
    usartPtr->readByte();
  }
  if ((this->flags & actOutgoingDataFlag) == actOutgoingDataFlag) {
    usartPtr->sendByte();
  }
}

ISR(USART_RX_vect) { usartPtr->flags |= actIncomingDataFlag; }

ISR(USART_TX_vect) { usartPtr->flags |= actOutgoingDataFlag; }
