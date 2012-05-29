#ifndef _SRC_BUS_H
#define _SRC_BUS_H

#include "uart.h"
#include "device.h"


#include "busprot.h"

#define N_BUSSES 4
#define BUS_TIMEOUT 250

enum {
        DT_MOTOR,
        DT_DUAL_MOTOR,
        DT_ACCU,
        DT_IPC,
};

struct bus_node
{
        struct bus_node* next;

        int devtype;
        uint16_t addr;
};

struct bus_descriptor
{
        struct uart_endpoint* uart;
        
        struct bus_node* layout;       
};

extern struct bus_descriptor bus[N_BUSSES];

void initialize_bus(void);
void bus_do_work(void);

#endif /* src/bus.h */
