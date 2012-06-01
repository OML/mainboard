#include "timer.h"
#include "device.h"
#include "oscillator.h"
#include "uart.h"

#include <stdlib.h>

#include <bus.h>

uint8_t u;

int main()
{
        clock_start();

        bus_init(N_UARTS, DT_ROOT);

        while(1) {
      	        bus_do_work();
        }

	return 0;
}
