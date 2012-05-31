#include "bus.h"

#include "timer.h"
#include "uart.h"

#include <stdlib.h>
#include <string.h>

struct bus_descriptor busses[N_BUSSES];

static void bus_send_packet(struct bus_descriptor* desc, const char* data, unsigned int length);
static void bus_read_packet(struct bus_descriptor* desc, char* data, unsigned int length);

static int send_hello(struct bus_descriptor* bus);

uint16_t addresses = 1;

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


static int send_hello(struct bus_descriptor* bus)
{
        volatile uint32_t start;
        
        struct bus_hdr* header;
        struct bus_hello* hello;
        struct bus_hello_reply* hello_reply;
        struct bus_node* node;


        int hello_reply_len = sizeof(struct bus_hdr) + sizeof(struct bus_hello_reply);
        char rx_buffer[hello_reply_len];
        hello_reply = get_buffer_hello_reply(rx_buffer);

        
        int hello_request_len = sizeof(struct bus_hdr) + sizeof(struct bus_hello);
        char tx_buffer[hello_request_len];
        hello = get_buffer_hello(tx_buffer);
        header = get_buffer_header(tx_buffer);



        header->opcode.op = BUSOP_HELLO;
        header->saddr = 0;
        header->daddr = ++addresses;
                
                         
        start = rt_timer();
        bus_send_packet(bus, tx_buffer, hello_request_len);
        while(rt_timer() - start < BUS_TIMEOUT) {
                if(uart_ep_bytes_available(bus->uart))
                        break;
        }
        
        if(uart_ep_bytes_available(bus->uart) == 0)
                return 0; // Timeout

        bus_read_packet(bus, rx_buffer, hello_reply_len);
                        
        node = (struct bus_node*)malloc(sizeof(struct bus_node));
        node->next = NULL;
        node->addr = addresses;
        node->devtype = hello_reply->devtype;

        bus_descriptor_add_node(bus, node);

        return 1;
}


void initialize_bus(void)
{
        int i;
        struct bus_descriptor* bus;
        while(rt_timer() < 2000); // wait 2 seconds

        for(i = 0; i < N_BUSSES; i++) {
                busses[i].layout = NULL;
                busses[i].uart = &uart[i];
                bus = &(busses[i]);
                while(send_hello(bus) != 0);
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

struct bus_hello_reply* get_buffer_hello_reply(const char* data)
{
        return (struct bus_hello_reply*)(((char*)get_buffer_header(data)) + sizeof(struct bus_hdr));
}



static void bus_send_packet(struct bus_descriptor* bus, const char* data, unsigned int len)
{
        uint16_t tot_len = len + sizeof(uint16_t);
        char tot_data[tot_len];
        memcpy((char*)&tot_data, &tot_len, sizeof(uint16_t));
        memcpy(((char*)&tot_data) + sizeof(uint16_t), data, len);
        uart_write(bus->uart, tot_data, tot_len); 
}

static void bus_read_packet(struct bus_descriptor* desc, char* data, unsigned int len)
{
        char buffer[len+sizeof(uint16_t)];
        uart_read(desc->uart, buffer, len+sizeof(uint16_t));
        
        memcpy(data, buffer+sizeof(uint16_t), len);
}




static void forward_packet(char* data, unsigned int len)
{
        int i;
        struct bus_node* node;
        struct bus_descriptor* dest_bus;
        struct bus_hdr* hdr = get_buffer_header(data + sizeof(uint16_t));

        dest_bus = NULL;
        for(i = 0; i < N_BUSSES && dest_bus == NULL; i++) {
                for(node = busses[i].layout; node; node = node->next) {
                        if(node->addr == hdr->daddr) {
                                dest_bus = &busses[i];
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
                src_bus = &busses[bus_id];

                if(uart_ep_bytes_available(src_bus->uart)) {
                        len = uart_ep_bytes_available(src_bus->uart);
                        buffer = (char*)malloc(len);
                        uart_read(src_bus->uart, buffer, len);


                        hdr = get_buffer_header(buffer + sizeof(uint16_t));
                        if(hdr->daddr == 0) {
                                if(hdr->opcode.op == BUSOP_ACQUIRE_ADDRESS)
                                        send_hello(src_bus);
                        }                   
                        else
                                forward_packet(buffer, len);  

                        free(buffer);
                }
        }
}

