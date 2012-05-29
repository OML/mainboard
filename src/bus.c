#include "bus.h"

#include "timer.h"

struct bus_descriptor bus[N_BUSSES];

void initialize_bus(void)
{
        int i;
        uint16_t addr = 0;
        volatile uint32_t start;
        struct bus_hello pa;

        for(i = 0; i < N_BUSSES; i++) {
                do {
                        bus[i].uart = &uart[i];
                        pa.opcode.op = BUSOP_HELLO;
                        pa.addr = addr++;
                
                        start = rt_timer();
                        uart_transmit((char*)&pa, sizeof(struct bus_hello));
                        while(rt_timer() - start < BUS_TIMEOUT) {
                                if(UART_EP_READY(uart[i].rx_buffer))
                                        break;
                        }
        
                        if(!UART_EP_READY(uart[i].rx_bufer)) {
                                break; // Timed out, no more devices
                        }
                }
        }
}

void bus_do_work(void)
{
}
