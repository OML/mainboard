#ifndef _BUSPROT_H
#define _BUSPROT_H


enum
{
        BUSOP_HELLO,
};

struct bus_opc
{
        uint8_t op;
} __attribute__((packed));

struct bus_hello
{
        struct bus_opc opcode;
        uint16_t addr;
} __attribute__((packed));

struct bus_hello_reply
{
        struct bus_opc opcode;
        uint16_t devtype;
};



#endif /* busprot.h */
