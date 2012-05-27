#ifndef _SRC_UART_H
#define _SRC_UART_H

#define BAUD 38400

#define BUF_LENGTH 32

struct uart_ep_buffer
{
        volatile char data[BUF_LENGTH];
        volatile int pos;
        volatile int length;
};

#define UART_EP_READY(ep) \
        (ep.pos == ep.length)

struct uart_endpoint
{ 
	volatile unsigned int* rcreg;
	volatile unsigned int* txreg;
        struct uart_ep_buffer rx_buffer;
        struct uart_ep_buffer tx_buffer;
};

extern struct uart_endpoint uart[4];

void initialize_uart_buffer(struct uart_ep_buffer* b);
void initialize_uarts(void);


void uart_transmit(const char* data, int length);

#endif /* src/uart.h */
