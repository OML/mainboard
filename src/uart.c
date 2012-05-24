#include "uart.h"
#include "device.h"

void init_rps(void)
{

	RPOR1bits.RP3R = 3;		//U1TX
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
}