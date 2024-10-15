#include "i2c_master.h"


void i2c_master_setup(struct i2c_bus_t *bus, long speed) {
    bus->twsr = 0;
    bus->twbr = ((F_CPU / speed) - 16) / 2 / (1);
    bus->twcr = BIT(TWEN) | BIT(TWEA);
}


static inline int i2c_start(struct i2c_bus_t *bus)
{
    bus->twcr = BIT(TWINT) | BIT(TWSTA) | BIT(TWEN);
    while ((bus->twcr & BIT(TWINT)) == 0);

    return (bus->twsr & 0xF8);;
}


static inline void i2c_stop(struct i2c_bus_t *bus)
{
    bus->twcr = BIT(TWINT) | BIT(TWSTO) | BIT(TWEN);
}


static inline int i2c_write_byte(struct i2c_bus_t *bus, uint8_t b)
{
    bus->twdr = b;
    bus->twcr = BIT(TWINT) | BIT(TWEN);
    while ((bus->twcr & BIT(TWINT)) == 0);

    return (bus->twsr & 0xF8);
}


static inline int i2c_read_byte(struct i2c_bus_t *bus, uint8_t *byte, bool ack)
{
    bus->twcr = BIT(TWINT) | BIT(TWEN) | (ack ? BIT(TWEA) : 0);
    while ((bus->twcr & BIT(TWINT)) == 0);
    
    *byte = bus->twdr;

    return (bus->twsr & 0xF8);
}

/**
 * @brief i2c_master_transaction sends or receives a data on i2c bus.
 * 
 * @param bus the bus instance
 * @param pkt the packet to send or receive
 * @return int 0 on success, a negative errno value otherwise
 */
int i2c_master_transaction(struct i2c_bus_t *bus, struct i2c_packet_t *pkt)
{
    size_t len = pkt->length;
    uint8_t sla = (pkt->slave_addr << 1) | ((pkt->dir == TW_READ) ? BIT(0) : 0);
    uint8_t *b;
    int status;

    status = i2c_start(bus);
    if (status != TW_START)
        return -22;

    if (pkt->subaddr_len > 0) {
        status = i2c_write_byte(bus, pkt->slave_addr << 1);
        if (status != TW_MT_SLA_ACK)
            return -19;
        
        b = &pkt->subaddr.subaddr_buf[0];
        while ((--pkt->subaddr_len) >= 0) {
            status = i2c_write_byte(bus, *b++);
            if (status != TW_MT_DATA_ACK)
                return -6;
        }

        if (pkt->dir == TW_READ) {
            status = i2c_start(bus);
            if (status != TW_REP_START)
                return -22;

            status = i2c_write_byte(bus, sla);
            if (status != TW_MR_SLA_ACK)
                return -19;

            goto read;       
        }
        else {
            goto write;
        }
    }
    else {
        status = i2c_write_byte(bus, sla);
        if (status != TW_MR_SLA_ACK && status != TW_MT_SLA_ACK)
            return -19;
        
        if (pkt->dir == TW_READ)
            goto read;
    }
write:
    b = &pkt->buffer[0];
    do {
        status = i2c_write_byte(bus, *b++);
        if (status != TW_MT_DATA_ACK)
            return -6;

    } while(len-- > 0);
    
read:
    b = &pkt->buffer[0];
    len = pkt->length;
    
    while(len-- > 0) {
        status = i2c_read_byte(bus, b, (len == 1));
        if (status != TW_MR_DATA_ACK && (status != TW_MR_DATA_ACK && len == 1))
            return -6;

        b++;
    }

    i2c_stop(bus);

    return 0;
}
