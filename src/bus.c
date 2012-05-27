#include "bus.h"

struct bus_descriptor bus[N_BUSSES];

void initialize_bus(void)
{
        int i;
        struct bus_packet;

        for(i = 0; i < N_BUSSES; i++) {
                bus[i].uart = &uart[i];
        }
}

void bus_do_work(void)
{
}
