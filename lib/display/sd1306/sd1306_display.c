#include "display.h"
#include "console.h"
#include "common.h"
#include <string.h>
#include <stdarg.h>

struct sd1306_cmd_t {
    int length;
    const uint8_t *cmd;
} __attribute__((__packed__));

#define SD1306_CMD(...) { \
    .length = sizeof((const uint8_t[]){__VA_ARGS__}), \
    .cmd = (const uint8_t[]){__VA_ARGS__}, \
}

static const struct sd1306_cmd_t xconst(sd1306_128_32_initcode[]) = {
    SD1306_CMD(SSD1306_SET_DISPLAY | 0), // display off
    SD1306_CMD(SSD1306_MEMORYMODE, 0x02), // Page Addressing mode
    SD1306_CMD(SSD1306_COMSCANDEC),             // Scan from 127 to 0 (Reverse scan)
    SD1306_CMD(SSD1306_SETSTARTLINE | 0x00),    // First line to start scanning from
    SD1306_CMD(SSD1306_SETCONTRAST, 0x7F),      // contast value to 0x7F according to datasheet
    SD1306_CMD(SSD1306_SEGREMAP | 0x01),        // Use reverse mapping. 0x00 - is normal mapping
    SD1306_CMD(SSD1306_NORMALDISPLAY),
    SD1306_CMD(SSD1306_SETMULTIPLEX, 63),       // Reset to default MUX. See datasheet
    SD1306_CMD(SSD1306_SETDISPLAYOFFSET, 0x00), // no offset
    SD1306_CMD(SSD1306_SETDISPLAYCLOCKDIV, 0x80),// set to default ratio/osc frequency
    SD1306_CMD(SSD1306_SETPRECHARGE, 0x22),     // switch precharge to 0x22 // 0xF1
    SD1306_CMD(SSD1306_SETCOMPINS, 0x12),       // set divide ratio
    SD1306_CMD(SSD1306_SETVCOMDETECT, 0x20),    // vcom deselect to 0x20 // 0x40
    SD1306_CMD(SSD1306_CHARGEPUMP, 0x14),       // Enable charge pump
    SD1306_CMD(SSD1306_DISPLAYALLON_RESUME),
    SD1306_CMD(SSD1306_SET_DISPLAY | 1),
};
#define SD1306_128_32_LEN ARRAY_SIZE(sd1306_128_32_initcode)

static const struct sd1306_cmd_t sd1306_128_64_initcode[] = {
    SD1306_CMD(SSD1306_SET_DISPLAY | 0), // display off
    SD1306_CMD(SSD1306_MEMORYMODE, 0x02), // Page Addressing mode
    SD1306_CMD(SSD1306_COMSCANDEC),             // Scan from 127 to 0 (Reverse scan)
    SD1306_CMD(SSD1306_SETSTARTLINE | 0x00),    // First line to start scanning from
    SD1306_CMD(SSD1306_SETCONTRAST, 0x7F),      // contast value to 0x7F according to datasheet
    SD1306_CMD(SSD1306_SEGREMAP | 0x01),        // Use reverse mapping. 0x00 - is normal mapping
    SD1306_CMD(SSD1306_NORMALDISPLAY),
    SD1306_CMD(SSD1306_SETMULTIPLEX, 63),       // Reset to default MUX. See datasheet
    SD1306_CMD(SSD1306_SETDISPLAYOFFSET, 0x00), // no offset
    SD1306_CMD(SSD1306_SETDISPLAYCLOCKDIV, 0x80),// set to default ratio/osc frequency
    SD1306_CMD(SSD1306_SETPRECHARGE, 0x22),     // switch precharge to 0x22 // 0xF1
    SD1306_CMD(SSD1306_SETCOMPINS, 0x12),       // set divide ratio
    SD1306_CMD(SSD1306_SETVCOMDETECT, 0x20),    // vcom deselect to 0x20 // 0x40
    SD1306_CMD(SSD1306_CHARGEPUMP, 0x14),       // Enable charge pump
    SD1306_CMD(SSD1306_DISPLAYALLON_RESUME),
    SD1306_CMD(SSD1306_SET_DISPLAY | 1),
};
#define SD1306_128_64_LEN ARRAY_SIZE(sd1306_128_64_initcode)

static inline int sd1306_cmd(struct display_bus_t *bus, uint8_t addr, const uint8_t cmd) {

    if (!bus || !bus->ops)
        return -1;
    
    return bus->ops->write(bus, addr, (uint8_t *)&cmd, 1);
}

void sd1306_display_init(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;
    const struct sd1306_cmd_t *initcode = NULL;
    int initcode_len = 0;
            
    if (!bus || !bus->ops)
        return;

    bus->ops->setup(bus);

    if (disp->height == 32) {
        initcode = sd1306_128_32_initcode;
        initcode_len = SD1306_128_32_LEN;
    }
    else if (disp->height == 64) {
        initcode = sd1306_128_64_initcode;
        initcode_len = SD1306_128_64_LEN;
    }
	
    for (int i = 0; i < initcode_len; ++i) {
		bus->ops->write(bus, 0, (uint8_t *)&initcode[i].cmd[0], initcode[i].length);
    }
}

void sd1306_display_power_off(struct display_t *disp) {
    sd1306_cmd(disp->bus, 0x0, SSD1306_SET_DISPLAY | 0);
}


void sd1306_display_power_on(struct display_t *disp) {
    sd1306_cmd(disp->bus, 0x0, SSD1306_SET_DISPLAY | 1);
}

void sd1306_display_invert(struct display_t *disp, bool mode) {
    sd1306_cmd(disp->bus, 0x0, mode ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}

void sd1306_display_flip_horizontal(struct display_t *disp, bool mode) {
    sd1306_cmd(disp->bus, 0x0, SSD1306_SEGREMAP | (mode ? 0 : 1));
}

void sd1306_display_flip_vertical(struct display_t *disp, bool mode) {
    sd1306_cmd(disp->bus, 0x0, mode ? SSD1306_COMSCANINC : SSD1306_COMSCANDEC);
}

void sd1306_display_clear_screen(struct display_t *disp) {

    screen_clear(disp->screen);

    disp->ops->draw_screen(disp);
}

void sd1306_display_set_brightness(struct display_t *disp, uint8_t value) {
    sd1306_cmd(disp->bus, 0x0, SSD1306_SETCONTRAST);
    sd1306_cmd(disp->bus, 0x0, value);
}

void sd1306_display_draw_screen(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;
    uint8_t *pixels = screen_get_buffer(disp->screen);
    if (!bus || !bus->ops)
        return;
    
    for (int y = 0; y < disp->height >> 3; ++y) {
        sd1306_cmd(bus, 0x0, 0xB0 + y); // set page address
        sd1306_cmd(bus, 0x00, 0x0); // set lower col
        sd1306_cmd(bus, 0x0, 0x10); // set upper col
        bus->ops->write(bus, 0x40, &pixels[disp->width * y], disp->width);
    }
}


struct display_ops_t sd1306_display_ops = {
    .init = sd1306_display_init,
    .power_off = sd1306_display_power_off,
    .power_on = sd1306_display_power_on,
	.set_brightness = sd1306_display_set_brightness,
    .clear = sd1306_display_clear_screen,
    .invert = sd1306_display_invert,
    .flip_horizontal = sd1306_display_flip_horizontal,
    .flip_vertical = sd1306_display_flip_vertical,
    .draw_screen = sd1306_display_draw_screen,
};
