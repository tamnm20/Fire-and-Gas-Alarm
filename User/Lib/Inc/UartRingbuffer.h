#ifndef UARTRINGBUFFER_H_
#define UARTRINGBUFFER_H_

#include "stm32f10x.h"

/* change the size of the buffer */
#define UART_BUFFER_SIZE 64

typedef struct
{
  unsigned char buffer[UART_BUFFER_SIZE];
  volatile unsigned int head;
  volatile unsigned int tail;
} ring_buffer;


/* Initialize the ring buffer */
void Ringbuf_init(void);

/* reads the data in the rx_buffer and increment the tail count in rx_buffer */
unsigned char Uart_read(void);

/* writes the data to the tx_buffer and increment the head count in tx_buffer */
void Uart_write(unsigned char c);

/* function to send the string to the uart */
void Uart_sendstring(const char *s);


/* Checks if the data is available to read in the rx_buffer */
int IsDataAvailable(void);


/* the ISR for the uart. put it in the IRQ handler */
void Uart_isr (void);

int IsFull(void);

#endif /* UARTRINGBUFFER_H_ */
