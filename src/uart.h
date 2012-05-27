#ifndef _SRC_UART_H
#define _SRC_UART_H

#define BAUD 38400

#define BUF_LENGTH 32


struct uart_endpoint
{ 
	volatile unsigned int* rcreg;
	volatile unsigned int* txreg;
	volatile char buffer[BUF_LENGTH];
	volatile int pos;
	volatile int exp_length;
};

extern struct uart_endpoint uart[4];

void initialize_uarts(void);

#endif /* src/uart.h */
