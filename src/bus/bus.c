#include "bus.h"

#include "timer.h"
#include "uart.h"

#include <stdlib.h>

struct bus_descriptor bus[N_BUSSES];

void bus_descriptor_add_node(struct bus_descriptor* desc, 
                                struct bus_node* node)
{
        struct bus_node* iter;

        if(desc->layout == NULL)  {
                desc->layout = node;
                return;
        }
        
        iter = desc->layout;
        while(iter->next != NULL)
                iter = iter->next;
        iter->next = node;
}


void initialize_bus(void)
{
        int i;
        uint16_t addr = 0;
        volatile uint32_t start;
        struct bus_hello hello;
        struct bus_hello_reply reply;

        struct bus_node* node;

        hello.hdr.opcode.op = BUSOP_HELLO;

        for(i = 0; i < N_BUSSES; i++) {
                bus[i].layout = NULL;
                bus[i].uart = &uart[i];
                do {
                        hello.hdr.addr = ++addr;
                
                        start = rt_timer();
                        uart_transmit(i, (char*)&hello, sizeof(struct bus_hello));
                        while(rt_timer() - start < BUS_TIMEOUT) {
                                if(UART_EP_READY(uart[i].rx_buffer))
                                        break;
                        }
        
                        if(!UART_EP_READY(uart[i].rx_buffer)) {
                                break; // Timed out, no more devices
                        }

                        uart_read(i, (char*)&reply, sizeof(struct bus_hello_reply));
                        
                        node = (struct bus_node*)malloc(sizeof(struct bus_node));
                        node->next = NULL;
                        node->addr = addr;
                        node->devtype = reply.devtype;
                } while(1);
        }
}

void bus_do_work(void)
{
}
