#include "uart.h"
#include "device.h"
#include "endian.h"
#include "oscillator.h"
#include <string.h>

struct uart_endpoint uart[4];

	

void _ISR __attribute__((no_auto_psv)) _U1RXInterrupt()
{  
	
	struct uart_ep_buffer* buf = &uart[0].rx_buffer;

	while(U1STAbits.URXDA == 1)
	{
		if(buf->pos == buf->length && buf->pos != 0)
			buf->pos = 0;

		buf->data[buf->pos] = U1RXREG;	

		if(buf->pos == 1)
			buf->length = le16toh(*(uint16_t*)&buf->data); //make this buffer an integer
		
		buf->pos++;
	}
	IFS0bits.U1RXIF = 0;
}

void _ISR __attribute__((no_auto_psv)) _U1TXInterrupt()
{
	IFS0bits.U1TXIF = 0;
}

void initialize_uart_buffer(struct uart_ep_buffer* buf)
{
        buf->pos = 0;
        buf->length = 0;
}


void init_buffers(void)
{
	int i;

        uart[0].rcreg = &U1RXREG;
	uart[1].rcreg = &U2RXREG;
        uart[2].rcreg = &U3RXREG;
	uart[3].rcreg = &U4RXREG;

	uart[0].txreg = &U1TXREG;
	uart[1].txreg = &U2TXREG;
	uart[2].txreg = &U3TXREG;
	uart[3].txreg = &U4TXREG;

	for(i = 0; i < 4; i++) {
                initialize_uart_buffer(&uart[i].tx_buffer);
                initialize_uart_buffer(&uart[i].rx_buffer);
        }       
}

void init_rps(void)
{
	RPOR1bits.RP3R = 3;				// U1TX
	TRISDbits.TRISD10 = 0;			// Configure as output
	RPINR18bits.U1RXR = 4;			// U1RX = RP4
	RPOR3bits.RP6R = 5;				// U2TX
	RPINR19bits.U2RXR = 7;			// U2RX
	RPOR4bits.RP8R = 28;	        // U3TX
	RPINR17bits.U3RXR = 9;			// U3RX
	RPOR5bits.RP10R = 30;	        // U4TX
	RPINR27bits.U4RXR = 17; 		// U4RX
}

void init_interrupts(void)
{
	IFS0bits.U1TXIF = 0;
	IFS0bits.U1RXIF = 0;
	IFS1bits.U2TXIF = 0;
	IFS1bits.U2RXIF = 0;
	IFS5bits.U3TXIF = 0;
	IFS5bits.U3RXIF = 0;
	IFS5bits.U4TXIF = 0;
	IFS5bits.U4RXIF = 0;

	IEC0bits.U1TXIE = 1;
	IEC0bits.U1RXIE = 1;
/*	IEC1bits.U2TXIE = 1;
	IEC1bits.U2RXIE = 1;
	IEC5bits.U3TXIE = 1;
	IEC5bits.U3RXIE = 1;
	IEC5bits.U4TXIE = 1;
	IEC5bits.U4RXIE = 1;*/
}

void initialize_uarts(void)
{
 	init_rps();
	init_buffers();
	init_interrupts();

	#define UMODEVAL 0x8000
	#define USTAVAL 0x0420
	#define BRG 51
	//25

	U1BRG = BRG;
	U2BRG = BRG;
	U3BRG = BRG;
	U4BRG = BRG;

	U1MODE = UMODEVAL;
	U2MODE = UMODEVAL;
	U3MODE = UMODEVAL;
	U4MODE = UMODEVAL;

	U1STA = USTAVAL;	
	U2STA = USTAVAL;
	U3STA = USTAVAL;
	U4STA = USTAVAL;






	#undef UMODEVAL
	#undef USTAVAL
	#undef BRG
}

void uart_write(struct uart_endpoint* ep, const char* data, int length)
{
	memcpy((char*)ep->tx_buffer.data, data, length); // put the data in the buffer
	int i;
	for(i = 0; i < length; i++)
	{
		while(U1STAbits.UTXBF == 1);
		*(ep->txreg) = data[i]; // write data to buffer
	}

}

void uart_read(struct uart_endpoint* ep, char* data, int length)
{
        memcpy(data, (char*)ep->rx_buffer.data, length);
        ep->rx_buffer.pos = ep->rx_buffer.length = 0;
}

unsigned int uart_ep_bytes_available(struct uart_endpoint* ep)
{
        if(ep->rx_buffer.pos == 0 || ep->rx_buffer.pos != ep->rx_buffer.length)
                return 0;
        return ep->rx_buffer.length;
}
