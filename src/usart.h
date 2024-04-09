#ifndef __usart_h
#define __usart_h

#include <avr/io.h>

#define bufferSize 32
#define sendingFlag (1 << 0)
#define recievingFlag (1 << 1)
#define standbyFlag (1 << 2)
#define incomingDataReadyFlag (1 << 3)
#define sendBufferClearFlag (1 << 4)
#define datalossFlag (1 << 5)

enum usartStatus {
  usartIsSending,
  usartIsRecieving,
  usartStandby,
  usartUndefined
};

class usart {
private:
  uint8_t flags;
  uint8_t sendIndex = 0;
  uint8_t sendBuffer[bufferSize];
  uint8_t recieveIndex = 0;
  uint8_t recieveBuffer[bufferSize];

public:
  usart(uint16_t baud);
  void sendByte(void);
  void sendString(const char *string);
  void readByte(void);
  bool incomingDataReady(void);
  bool sendBufferClear(void);
  void readData(char *string);
  usartStatus getUsartStatus(void);
};

uint8_t decodeIncomingAmount(const char *string);
uint8_t decodeMessage(const char *string);
uint8_t decodeMultiplier(uint8_t loopCount);

extern usart *usartPtr;

#endif // !__usart_h