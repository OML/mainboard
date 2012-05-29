#include "timer.h"
#include "device.h"
#include "oscillator.h"
#include "uart.h"
#include "bus.h"

int main()
{
        configure_oscillator();
        timer_start();

        initialize_uarts();

        initialize_bus();
	
        while(1) {
				
               if(UART_EP_READY(uart[0].rx_buffer))
					{
						uart_transmit(0, (const char*)uart[0].rx_buffer.data, (int)uart[0].rx_buffer.length);
					}
        }

	return 0;
}
