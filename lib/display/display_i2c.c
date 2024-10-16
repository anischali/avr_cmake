#include "display.h"
#include "i2c_master.h"

void display_i2c_setup(struct display_bus_t *disp) {
    struct i2c_bus_t *i2c = (struct i2c_bus_t *)disp->bus;
    struct display_i2c_cfg_t *cfg = (struct display_i2c_cfg_t *)disp->context;

    i2c_master_setup(i2c, cfg->speed);
}

int display_i2c_write(struct display_bus_t *disp, uint32_t addr, uint8_t *buffer, size_t len) {
    static struct i2c_packet_t pkt;
    struct i2c_bus_t *i2c = (struct i2c_bus_t *)disp->bus;
    struct display_i2c_cfg_t *cfg = (struct display_i2c_cfg_t *)disp->context;
    
    i2c_packet_fill(&pkt, TW_WRITE, cfg->address, addr, ADDR_SIZE(addr), buffer, len);

    return i2c_master_transaction(i2c, &pkt);
}


int display_i2c_raw_write(struct display_bus_t *disp, uint8_t *buffer, size_t len) {
    static struct i2c_packet_t pkt;
    struct i2c_bus_t *i2c = (struct i2c_bus_t *)disp->bus;
    struct display_i2c_cfg_t *cfg = (struct display_i2c_cfg_t *)disp->context;
    
    i2c_packet_fill(&pkt, TW_WRITE, cfg->address, 0, 0, buffer, len);

    return i2c_master_transaction(i2c, &pkt);
}


struct display_bus_ops_t i2c_display_ops = {
    .setup = display_i2c_setup,
    .write = display_i2c_write,
    .raw_write = display_i2c_raw_write,
};