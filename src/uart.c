#include "uart.h"
#include "device.h"
#include "oscillator.h"

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

	U1MODEbits.UARTEN = 0;
	U2MODEbits.UARTEN = 0;
	U3MODEbits.UARTEN = 0;
	U4MODEbits.UARTEN = 0;

	U1BRG = U2BRG = U3RBG = U4BRG = (FCY / (16 * BAUD))-1;

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