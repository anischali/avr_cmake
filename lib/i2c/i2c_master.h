#ifndef __I2C_MASTER_H_
#define __I2C_MASTER_H_
#include "i2c.h"
#include <avr/io.h>
#include "common_avr.h"
#include <errno.h>

#define i2c_wait_ready(bus) while (!(bus->TTWCR & BIT(TWINT)))
#define i2c_status(bus) (bus->TTWSR & I2C_STATUS_Msk)
static inline int i2c_master_start(struct i2c_bus_t *bus) {
    bus->TTWCR = (BIT(TWINT) | BIT(TWEN) | BIT(TWSTA));
    i2c_wait_ready(bus);
    if (i2c_status(bus) != I2C_TWSR_START) {
        return -ENXIO; \
    }
    return 0;
}

#define i2c_send_byte(bus, b) bus->TTWDR = b; bus->TTWCR = BIT(TWINT) | BIT(TWEN);
#define i2c_master_stop(bus) ((bus)->TTWCR = (BIT(TWINT) | BIT(TWEN) | BIT(TWSTO)))

static inline void i2c_recv_byte(struct i2c_bus_t *bus, uint8_t *b) {
    *(b) = bus->TTWDR;
}


static inline int i2c_send_data_byte(struct i2c_bus_t *bus, uint8_t b) {
    
    i2c_send_byte(bus, b);
    i2c_wait_ready(bus);
    if (i2c_status(bus) != I2C_TWSR_DATA_ACK) {
        return -ENXIO;
    }

    return 0;
}

static inline int i2c_send_slave_addr(struct i2c_bus_t *bus, uint8_t sla) {
    
    i2c_send_byte(bus, sla);
    i2c_wait_ready(bus);
    if (i2c_status(bus) != I2C_TWSR_ADDR_ACK) {
        return -ENODEV; 
    }

    return 0;
}

static inline void i2c_master_setup(struct i2c_bus_t *bus, int32_t speed) {
    
    uint8_t pres, twbr;

    switch (speed)
    {
    case 400000:
        pres = 0x1; // 4 prescaler
        twbr = 0x3;
        break;
    case 100000:
        pres = 0x1;
        twbr = 0x12;
        break;
    case 50000:
        pres = 0x1;
        twbr = 0x26;
        break;
    case 10000:
        pres = 0x1;
        twbr = 0xC6;
        break;
    
    default:
        break;
    }

    bus->TTWBR = twbr;
    bus->TTWSR |= ((BIT(TWPS1) | BIT(TWPS0)) & pres);
};

/**
 * @brief i2c_master_transaction sends or receives a data on i2c bus.
 * 
 * @param bus the bus instance
 * @param pkt the packet to send or receive
 * @return int 0 on success, a negative errno value otherwise
 */
static inline int i2c_master_transaction(struct i2c_bus_t *bus, struct i2c_packet_t *pkt)
{
    int status;
    size_t len = pkt->length;
    uint8_t sla = (pkt->slave_addr << 1) | ((pkt->dir == I2C_READ) ? BIT(0) : 0);
    uint8_t *b = &pkt->buffer[0], *sa =  pkt->subaddr.subaddr_buf;

    status = i2c_master_start(bus);
    if (status)
        return status;
    
    if (pkt->subaddr_len > 0) {
        status = i2c_send_slave_addr(bus, pkt->slave_addr);
        if (status)
            return status;

        while ((--pkt->subaddr_len) >= 0) {
            status = i2c_send_data_byte(bus, *sa++);
            if (status)
                goto stop;
        }

        if (pkt->dir == I2C_READ) {
            status = i2c_master_start(bus);
            if (status)
                goto stop;

            status = i2c_send_slave_addr(bus, sla);
            if (status)
                return status;
            
            goto read;       
        }
        else {
            goto write;
        }
    }
    else {
        status = i2c_send_slave_addr(bus, sla);
        if (status)
            return status;
        
        if (pkt->dir == I2C_READ)
            goto read;
    }
write:
    do {
        status = i2c_send_data_byte(bus, *(b++));
        if (status)    
            goto stop;

    } while(--len > 0);
    
read:
    do {
        i2c_recv_byte(bus, b);
        b++;
    } while(--len > 0);

stop:
    i2c_master_stop(bus);

    return 0;
}

#endif