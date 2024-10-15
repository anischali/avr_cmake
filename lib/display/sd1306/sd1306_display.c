#ifndef __SD1306_CONTROLLER_H__
#define __SD1306_CONTROLLER_H__
#include "display.h"
#include "console.h"

static uint8_t sd1306_initcode[] = {
    0xAE, //display off
	0x20, //Set Memory Addressing Mode   
	0x10, //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	0xB0, //Set Page Start Address for Page Addressing Mode,0-7
	0xC8, //Set COM Output Scan Direction
	0x00, //---set low column address
	0x10, //---set high column address
	0x40, //--set start line address
	0x81, //--set contrast control register
	0xFF,
	0xA1, //--set segment re-map 0 to 127
	0xA6, //--set normal display
	0xA8, //--set multiplex ratio(1 to 64)
	0x3F, //
	0xA4, //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	0xD3, //-set display offset
	0x00, //-not offset
	0xD5, //--set display clock divide ratio/oscillator frequency
	0xF0, //--set divide ratio
	0xD9, //--set pre-charge period
	0x22, //
	0xDA, //--set com pins hardware configuration
	0x12,
	0xDB, //--set vcomh
	0x20, //0x20,0.77xVcc
	0x8D, //--set DC-DC enable
	0x14, //
	0xAF, //--turn on SSD1306 panel
	0x29, // Scroll command
    0x2E, // deactivate scroll
};

void sd1306_display_init(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;

    if (!bus || !bus->ops)
        return;
    
    bus->ops->setup(bus);
    
	bus->ops->write(bus, &sd1306_initcode[0], sizeof(sd1306_initcode));
}



void sd1306_display_power_off(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;
    uint8_t cmd = 0xAE;

    if (!bus || !bus->ops)
        return;
    
    bus->ops->write(bus, &cmd, sizeof(cmd));

}


void sd1306_display_power_on(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;
    uint8_t cmd = 0xAF;

    if (!bus || !bus->ops)
        return;
    
    bus->ops->write(bus, &cmd, sizeof(cmd));
}


void sd1306_display_set_brightness(struct display_t *disp, uint8_t value) {
    struct display_bus_t *bus = disp->bus;
    uint8_t cmd[2] = { 0x81, value };

    if (!bus || !bus->ops)
        return;
    
    bus->ops->write(bus, &cmd[0], sizeof(cmd));
}



struct display_ops_t sd1306_display_ops = {
    .init = sd1306_display_init,
    .power_off = sd1306_display_power_off,
    .power_on = sd1306_display_power_on,
	.set_brightness = sd1306_display_set_brightness,
};

#endif