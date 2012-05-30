#include "bus.h"

#include "timer.h"
#include "uart.h"

#include <stdlib.h>
#include <string.h>

struct bus_descriptor bus[N_BUSSES];

void bus_descriptor_add_node(struct bus_descriptor* desc, 
                                struct bus_node* new_node)
{
        struct bus_node* iter;

        if(desc->layout == NULL)  {
                desc->layout = new_node;
                return;
        }
        
        iter = desc->layout;
        while(iter->next != NULL)
                iter = iter->next;
        iter->next = new_node;
}

static void busprot_transmit(struct uart_endpoint* ep, const char* data, size_t pack_len)
{
        uint16_t data_len = pack_len + sizeof(uint16_t);
        char buffer[data_len];
        memcpy(buffer, (char*)&data_len, sizeof(uint16_t));
        memcpy(buffer + sizeof(uint16_t), data, pack_len);
        
        uart_write(ep, buffer, data_len);
}

struct bus_node ipc_node;
struct bus_node* ipc_ptr;

void initialize_bus(void)
{
        int i;
        uint16_t addr = 0;
        volatile uint32_t start;
        
        struct bus_hdr* header;
        struct bus_hello* hello;
        struct bus_hello_reply* reply;
        
        char rx_buffer[sizeof(struct bus_hdr) + sizeof(bus_hello_reply)];
        char buffer[sizeof(struct bus_hdr) + sizeof(struct bus_hello)];
        hello = get_buffer_hello(buffer);
        header = get_buffer_header(buffer);

        struct bus_node* node;

        header->opcode.op = BUSOP_HELLO;

        for(i = 0; i < N_BUSSES; i++) {
                bus[i].layout = NULL;
                bus[i].uart = &uart[i];
                do {
                        header->saddr = 0;
                        header->daddr = ++addr;
                
                        start = rt_timer()
;
                        busprot_transmit(bus[i].uart, (char*)&hello, sizeof(struct bus_hello));
                        while(rt_timer() - start < BUS_TIMEOUT) {
                                if(uart_ep_bytes_available(bus[i].uart))
                                        break;
                        }
        
                        if(uart_ep_bytes_available(bus[i].uart) == 0) {
                                break; // Timed out, no more devices
                        }

                        uart_read(bus[i].uart, (char*)&reply, sizeof(struct bus_hello_reply) + sizeof(uint16_t));
                        
                        node = (struct bus_node*)malloc(sizeof(struct bus_node));
                        node->next = NULL;
                        node->addr = addr;
                        node->devtype = reply.devtype;

                        ipc_node = *node;
                        ipc_ptr = node;

                        bus_descriptor_add_node(&bus[i], node);
                } while(1);
        }
}

struct bus_hdr* get_buffer_header(const char* data)
{
        return (struct bus_hdr*)(data);
}

struct bus_hello* get_buffer_hello(const char* data)
{
        return (struct bus_hello*)(((char*)get_buffer_header(data)) + sizeof(struct bus_hdr));
}


static void forward_packet(char* data, unsigned int len)
{
        int i;
        struct bus_node* node;
        struct bus_descriptor* dest_bus;
        struct bus_hdr* hdr = get_buffer_header(data + sizeof(uint16_t));

        dest_bus = NULL;
        for(i = 0; i < N_BUSSES && dest_bus == NULL; i++) {
                for(node = bus[i].layout; node; node = node->next) {
                        if(node->addr == hdr->daddr) {
                                dest_bus = &bus[i];
                                break;
                        }
                }
        }

        if(dest_bus)
                uart_write(dest_bus->uart, data, len);

}

void bus_do_work(void)
{
        int bus_id;
        int len;
        char* buffer;
        struct bus_hdr* hdr;
        struct bus_descriptor* src_bus;

        for(bus_id = 0; bus_id < N_BUSSES; bus_id++) {
                src_bus = &bus[bus_id];

                if(uart_ep_bytes_available(src_bus->uart)) {
                        len = uart_ep_bytes_available(src_bus->uart);
                        buffer = (char*)malloc(len);
                        uart_read(src_bus->uart, buffer, len);


                        hdr = get_buffer_header(buffer + sizeof(uint16_t));
                        if(hdr->daddr == 0) {
                        }                   
                        else
                                forward_packet(buffer, len);  

                        free(buffer);
                }
        }
}

