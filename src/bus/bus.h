#ifndef _SRC_BUS_H
#define _SRC_BUS_H

#include "uart.h"
#include "device.h"


#include "busprot.h"

#define N_BUSSES 4
#define BUS_TIMEOUT 250

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

void bus_descriptor_add_node(struct bus_descriptor* desc, 
                                struct bus_node* node);





extern struct bus_descriptor busses[N_BUSSES];

void initialize_bus(void);
void bus_do_work(void);


struct bus_hdr* get_buffer_header(const char* data);
struct bus_hello* get_buffer_hello(const char* data);
struct bus_hello_reply* get_buffer_hello_reply(const char* data);


#endif /* src/bus.h */
