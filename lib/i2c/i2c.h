#ifndef __I2C_H_
#define __I2C_H_
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_avr.h"

#define I2C_STATUS_Msk                      0xF8
#define I2C_PRESCALER_Msk                   0x03
#define I2C_SLAVE_ADDR_Msk                  0xFE
#define I2C_SLAVE_ADDR_GC_RECON_Msk         0x01
#define I2C_SLAVE_ADDR_MASK_Msk             0xFE

enum I2C_TWSR_STATUS {
    I2C_TWSR_START = 0x8,
    I2C_TWSR_REPEATED_START = 0x10,
    I2C_TWSR_ADDR_ACK = 0x18,
    I2C_TWSR_ADDR_NACK = 0x20,
    I2C_TWSR_DATA_ACK = 0x28,
    I2C_TWSR_DATA_NACK = 0x30,
    I2C_TWSR_ARBTR_LOST = 0x38
};

typedef union {
    uint32_t subaddr;
    uint8_t subaddr_buf[4];
} subaddr_t;

enum {
    I2C_WRITE, I2C_READ
};

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
    __IO uint8_t TTWBR;             // bitrate register
    __IO uint8_t TTWSR;             // status register
    __IO uint8_t TTWAR;             // slave address register
    __IO uint8_t TTWDR;             // data register
    __IO uint8_t TTWCR;             // control register
    __IO uint8_t TTWAMR;            // slave address mask register
};

#define I2C0    ((struct i2c_bus_t *)(TWBR))


#endif