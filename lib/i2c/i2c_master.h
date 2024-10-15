#ifndef __I2C_MASTER_H_
#define __I2C_MASTER_H_
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include "common_avr.h"

typedef union {
    uint32_t subaddr;
    uint8_t subaddr_buf[4];
} subaddr_t;

struct i2c_packet_t {
    int dir;
    uint8_t slave_addr;
    subaddr_t subaddr;
    int subaddr_len;
    uint8_t *buffer;
    size_t length;
};

#define i2c_packet_fill(packet, op, slave, _subaddr, _subaddr_len, buf, len) \
    (packet)->dir = op; \
    (packet)->slave_addr = slave; \
    (packet)->subaddr.subaddr = _subaddr; \
    (packet)->subaddr_len = _subaddr_len; \
    (packet)->buffer = buf; \
    (packet)->length = len


struct i2c_bus_t {
    __IO uint8_t twbr;             // bitrate register
    __IO uint8_t twsr;             // status register
    __IO uint8_t twar;             // slave address register
    __IO uint8_t twdr;             // data register
    __IO uint8_t twcr;             // control register
    __IO uint8_t twamr;            // slave address mask register
};

#define I2C0    ((struct i2c_bus_t *)(0xB8U))

void i2c_master_setup(struct i2c_bus_t *bus, long speed);
int i2c_master_transaction(struct i2c_bus_t *bus, struct i2c_packet_t *pkt);

#endif