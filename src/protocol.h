#ifndef _SRC_PROTOCOL_H
#define _SRC_PROTOCOL_H

#include <stdint.h>

typedef uint8_t opcode_t;

typedef int16_t throttle_t;
typedef uint16_t voltage_t;
typedef uint16_t current_t;
typedef int16_t temperature_t;
typedef int16_t gyro_t;

struct thresholds
{
        throttle_t throttle;
        voltage_t voltage;
        current_t current;
        temperature_t temperature;
        gyro_t gyro;
};

enum 
{
        OP_HELLO = 0,
        OP_EVENT,
};

struct packet_mtodev
{
        opcode_t opcode;
        union {
                struct {
                        thresholds th;
                } __attribute__((packed)) hello;
                struct {
                        throttle_t throttle;
                        uint32_t flags;
                } __attribute__((packed)) event;
        };
} __attribute__((packed));

#endif /* src/protocol.h */
