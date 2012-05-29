#include "bus.h"

#include "timer.h"

#include <stdlib.h>

struct bus_descriptor bus[N_BUSSES];

void initialize_bus(void)
{
        int i;
        uint16_t addr = 0;
        volatile uint32_t start;
        struct bus_hello pa;
        struct bus_node* node;

        for(i = 0; i < N_BUSSES; i++) {
                bus[i].layout = NULL;
                bus[i].uart = &uart[i];
                do {
                        pa.opcode.op = BUSOP_HELLO;
                        pa.addr = addr++;
                
                        start = rt_timer();
                        uart_transmit(i, (char*)&pa, sizeof(struct bus_hello));
                        while(rt_timer() - start < BUS_TIMEOUT) {
                                if(UART_EP_READY(uart[i].rx_buffer))
                                        break;
                        }
        
                        if(!UART_EP_READY(uart[i].rx_buffer)) {
                                break; // Timed out, no more devices
                        }

                        node = (struct bus_node*)malloc(sizeof(struct bus_node));
                } while(1);
        }
}

void bus_do_work(void)
{
}
