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

struct uart_endpoint
{ 
        volatile unsigned int* rcreg;
        volatile unsigned int* txreg;
        volatile unsigned int* stareg;
        struct uart_ep_buffer rx_buffer;
        struct uart_ep_buffer tx_buffer;
};

extern struct uart_endpoint uart[4];

void initialize_uart_buffer(struct uart_ep_buffer* b);
void initialize_uarts(void);


void uart_write(struct uart_endpoint* ep, const char* data, int length);
void uart_read(struct uart_endpoint* ep, char* data, int length);

unsigned int uart_ep_bytes_available(struct uart_endpoint* ep);

#endif /* src/uart.h */
