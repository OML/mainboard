#include "device.h"
#include "oscillator.h"
#include "uart.h"
#include "bus.h"
#include "timer.h"

int main()
{
	configure_oscillator();
        timer_start();

	initialize_uarts();

        initialize_bus();
	
        while(1) {
                bus_do_work();
        }

	return 0;
}
