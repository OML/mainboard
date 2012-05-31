#include "uart.h"
#include "device.h"
#include "endian.h"
#include "oscillator.h"

#include <bus.h>

struct uart_descriptor* uarts[N_UARTS];

void uart_init(struct uart_descriptor* uart, size_t uid)
{
        uarts[uid] = uart;

	#define UMODEVAL 0x8000
	#define USTAVAL 0x0420
	#define BRG 51

        switch(uid) {
                case 0:
	                RPOR1bits.RP3R = 3;			// U1TX
	                TRISDbits.TRISD10 = 0;			// Configure as output
	                RPINR18bits.U1RXR = 4;			// U1RX = RP4

	                IFS0bits.U1TXIF = 0;
	                IFS0bits.U1RXIF = 0;
	
                        IEC0bits.U1TXIE = 1;
	                IEC0bits.U1RXIE = 1;

	                U1BRG = BRG;
	                U1MODE = UMODEVAL;
	                U1STA = USTAVAL;
                        break;

                case 1:
	                RPOR3bits.RP6R = 5;			// U2TX
	                RPINR19bits.U2RXR = 7;			// U2RX
                        TRISFbits.TRISF8 = 0;                   // Configure as output
                       
	                IFS1bits.U2TXIF = 0;
                	IFS1bits.U2RXIF = 0;

                	IEC1bits.U2TXIE = 1;
                	IEC1bits.U2RXIE = 1;

	                U2BRG = BRG;
	                U2MODE = UMODEVAL;
	                U2STA = USTAVAL;
                        break;

                case 2:
                       	RPOR4bits.RP8R = 28;	                // U3TX
                       	RPINR17bits.U3RXR = 9;			// U3RX
                        TRISBbits.TRISB8;                       // Idem

                        IFS5bits.U3TXIF = 0;
                        IFS5bits.U3RXIF = 0;

                        IEC5bits.U3TXIE = 1;
	                IEC5bits.U3RXIE = 1;

                        U3BRG = BRG;
	                U3MODE = UMODEVAL;
	                U3STA = USTAVAL;
                        break;
        
                case 3:
                        RPOR5bits.RP10R = 30;	                // U4TX
	                RPINR27bits.U4RXR = 17; 		// U4RX
                        TRISFbits.TRISF4 = 0;                   // Idem

                        IFS5bits.U4TXIF = 0;
                	IFS5bits.U4RXIF = 0;

                        IEC5bits.U4TXIE = 1;
	                IEC5bits.U4RXIE = 1;

                        U4BRG = BRG;
	                U4MODE = UMODEVAL;
	                U4STA = USTAVAL;
                        break;
                default:
                        break;
        }

	#undef UMODEVAL
	#undef USTAVAL
	#undef BRG
}


void _ISR __attribute__((no_auto_psv)) _U1RXInterrupt()
{  
        uart_has_byte_available(uarts[0]);
	IFS0bits.U1RXIF = 0;
}

void _ISR __attribute__((no_auto_psv)) _U1TXInterrupt()
{
	IFS0bits.U1TXIF = 0;
}



void _ISR __attribute__((no_auto_psv)) _U2RXInterrupt()
{  
        uart_has_byte_available(uarts[1]);
	IFS1bits.U2RXIF = 0;
}

void _ISR __attribute__((no_auto_psv)) _U2TXInterrupt()
{
	IFS1bits.U2TXIF = 0;
}



void _ISR __attribute__((no_auto_psv)) _U3RXInterrupt()
{  
        uart_has_byte_available(uarts[2]);
	IFS5bits.U3RXIF = 0;
}

void _ISR __attribute__((no_auto_psv)) _U3TXInterrupt()
{
	IFS5bits.U3TXIF = 0;
}



void _ISR __attribute__((no_auto_psv)) _U4RXInterrupt()
{  
        uart_has_byte_available(uarts[3]);
	IFS5bits.U4RXIF = 0;
}

void _ISR __attribute__((no_auto_psv)) _U4TXInterrupt()
{
	IFS5bits.U4TXIF = 0;
}

