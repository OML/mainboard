#include "device.h"
#include "oscillator.h"
#include "uart.h"

int main()
{
	configure_oscillator();
	initialize_uarts();
	
	return 0;
}
