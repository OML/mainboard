#ifndef _SRC_BUS_H
#define _SRC_BUS_H

#include "uart.h"

#include <stdint.h>

#define N_BUSSES 4

enum {
        DT_MOTOR,
        DT_DUAL_MOTOR,
        DT_ACCU,
};

struct bus_node
{
        struct bus_node* next;

        int devid;
        int devtype;
};

struct bus_descriptor
{
        struct uart_endpoint* uart;
        
        struct bus_node* layout;       
};

extern struct bus_descriptor bus[N_BUSSES];

void initialize_bus(void);
void bus_do_work(void);


enum
{
        BUSOP_HELLO,
};

struct bus_opc
{
        uint8_t opcode;
} __attribute__((packed));

#endif /* src/bus.h */
