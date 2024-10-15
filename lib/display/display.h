#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include <stdint.h>
#include "list.h"
#include "sd1306_display.h"

struct display_t;
struct display_zone_t;
struct display_bus_t;

struct point_t {
    int x;
    int y;
};

struct display_zone_t {
    struct point_t start;
    struct point_t end;
    struct display_t *display;
    void *buffer;

    void (*update)(struct display_zone_t *handle);
};


struct display_ops_t {
    void (*init)(struct display_t *disp);
    void (*update)(struct display_t *disp);
    void (*clear)(struct display_t *disp);
    void (*power_on)(struct display_t *disp);
    void (*power_off)(struct display_t *disp);
    void (*set_brightness)(struct display_t *disp, uint8_t value);
};

struct display_i2c_cfg_t {
    int address;
    long speed;
};

struct display_bus_ops_t {
    void (*setup)(struct display_bus_t *bus);
    int (*write)(struct display_bus_t *bus, uint8_t *buffer, size_t size);
    int (*read)(struct display_bus_t *bus, uint8_t *buffer, size_t size);
};

struct display_bus_t {
    void *bus;
    void *context;
    struct display_bus_ops_t *ops;
};

struct display_t {
    int height;
    int width;
    void *buffer;
    void *context;
    struct list_head zones;
    struct display_bus_t *bus;
    struct display_ops_t *ops;
};

extern struct display_bus_ops_t i2c_display_ops;

#define DEFINE_I2C_DISPLAY(name, _height, _width, _bus_intf, _addr, _speed, _bus_ops, _disp_ops) \
    static struct display_i2c_cfg_t name##_cfg = { \
        .address = _addr, \
        .speed = _speed, \
    }; \
    static struct display_bus_t name##_bus = { \
        .bus = _bus_intf, \
        .context = &name##_cfg, \
        .ops = _bus_ops, \
    }; \
    static struct display_t name = { \
        .height = _height, \
        .width = _width, \
        .bus = &name##_bus, \
        .ops = _disp_ops, \
    }


void display_init(struct display_t *disp);
void display_power_off(struct display_t *disp);
void display_power_on(struct display_t *disp);
void display_set_brightness(struct display_t *disp, uint8_t value);

#endif