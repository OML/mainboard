#include "bus.h"

struct bus_descriptor bus[N_BUSSES];

void initialize_bus(void)
{
        int i;
        uint16_t addr = 0;
        struct bus_hello pa;

        for(i = 0; i < N_BUSSES; i++) {
                bus[i].uart = &uart[i];
                pa.opcode.op = BUSOP_HELLO;
                pa.addr = addr++;
                
                uart_transmit((char*)&pa, sizeof(struct bus_hello));

                
        }
}

void bus_do_work(void)
{
}
