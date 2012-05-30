#ifndef _SRC_UART_H
#define _SRC_UART_H

#define BAUD 38400

#define BUF_LENGTH 64


struct uart_ep_buffer
{
        volatile char data[BUF_LENGTH];
        volatile int pos;
        volatile int length;
};

#define UART_EP_READY(ep) \
        (((ep).pos == (ep).length) && (ep).pos != 0)

#define UART_RX_BUFFER_HAS_DATA(uart) \
        (UART_EP_READY((uart).rx_buffer))

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


void uart_transmit(int uid, const char* data, int length);
void uart_read(int uid, char* data, int length);

#endif /* src/uart.h */
