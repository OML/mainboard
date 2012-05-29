#ifndef _BUSPROT_H
#define _BUSPROT_H

#include "device.h"

typedef uint16_t bus_addr_t;
typedef uint16_t bus_devtype_t;
typedef uint16_t bus_event_type_t;

enum
{
        BUSOP_HELLO = 0,
        BUSOP_EVENT,
};

struct bus_opc
{
        uint8_t op;
} __attribute__((packed));

struct bus_hello
{
        struct bus_opc opcode;
        bus_addr_t addr;
} __attribute__((packed));

struct bus_hello_reply
{
        struct bus_opc opcode;
        bus_devtype_t devtype;
};

struct bus_event_header
{
        struct bus_opc opcode;
        bus_addr_t addr;

        bus_event_type_t event_type;
};

#endif /* busprot.h */
