#include "uart.h"
#include "device.h"
#include "oscillator.h"

#define BUF_LENGTH 32

struct {
	struct { 
		volatile unsigned int* rcreg;
		volatile unsigned int* txreg;
		volatile char buffer[BUF_LENGTH];
		volatile int pos;
		volatile int exp_length;
	} uart[4];
} buffers;
	

void init_buffers(void)
{
	int i;

	buffers.uart[0].rcreg = &U1RXREG;
	buffers.uart[1].rcreg = &U2RXREG;
	buffers.uart[2].rcreg = &U3RXREG;
	buffers.uart[3].rcreg = &U4RXREG;

	buffers.uart[0].txreg = &U1TXREG;
	buffers.uart[1].txreg = &U2TXREG;
	buffers.uart[2].txreg = &U3TXREG;
	buffers.uart[3].txreg = &U4TXREG;

	for(i = 0; i < 4; i++)
		buffers.uart[i].pos = 0;
}

void init_rps(void)
{
	RPOR1bits.RP3R = 3;		// U1TX
	RPINR18 = 4;			// U1RX
	RPOR3bits.RP6R = 5;		// U2TX
	RPINR19 = 7;			// U2RX
	RPOR4bits.RP8R = 28;	// U3TX
	RPINR17 = 9;			// U3RX
	RPOR5bits.RP10R = 30;	// U4TX
	RPINR27 = 17; 			// U4RX
}

void initialize_uarts(void)
{
 	init_rps();
	init_buffers();

	U1MODEbits.UARTEN = 0;
	U2MODEbits.UARTEN = 0;
	U3MODEbits.UARTEN = 0;
	U4MODEbits.UARTEN = 0;

	U1BRG = U2BRG = U3BRG = U4BRG = (FCY / (16 * BAUD))-1;

	U1STAbits.URXISEL = 2;
	U2STAbits.URXISEL = 2;
	U3STAbits.URXISEL = 2;
	U4STAbits.URXISEL = 2;
	
	U1STAbits.UTXEN = 1;
	U2STAbits.UTXEN = 1;
	U3STAbits.UTXEN = 1;
	U4STAbits.UTXEN = 1;

	U1MODEbits.UARTEN = 1;
	U2MODEbits.UARTEN = 1;
	U3MODEbits.UARTEN = 1;
	U4MODEbits.UARTEN = 1;
}
