#include "display.h"
#include "console.h"
#include "common.h"
#include <string.h>
#include <stdarg.h>

struct sd1306_cmd_t {
    int length;
    uint8_t cmd[6];
};

static struct sd1306_cmd_t xconst sd1306_128_32_initcode[] = {
    { .length = 1, .cmd = { SSD1306_SET_DISPLAY | 0} }, // display off
    { .length = 2, .cmd = { SSD1306_MEMORYMODE, 0x00} }, // Page Addressing mode
    { .length = 1, .cmd = { SSD1306_COMSCANDEC} },             // Scan from 127 to 0 (Reverse scan)
    { .length = 1, .cmd = { SSD1306_SETSTARTLINE | 0x00} },    // First line to start scanning from
    { .length = 2, .cmd = { SSD1306_SETCONTRAST, 0x7F} },      // contast value to 0x7F according to datasheet
    { .length = 1, .cmd = { SSD1306_SEGREMAP | 0x01} },        // Use reverse mapping. 0x00 - is normal mapping
    { .length = 1, .cmd = { SSD1306_NORMALDISPLAY} },
    { .length = 2, .cmd = { SSD1306_SETMULTIPLEX, 63} },       // Reset to default MUX. See datasheet
    { .length = 2, .cmd = { SSD1306_SETDISPLAYOFFSET, 0x00} }, // no offset
    { .length = 2, .cmd = { SSD1306_SETDISPLAYCLOCKDIV, 0x80} },// set to default ratio/osc frequency
    { .length = 2, .cmd = { SSD1306_SETPRECHARGE, 0x22} },     // switch precharge to 0x22 // 0xF1
    { .length = 2, .cmd = { SSD1306_SETCOMPINS, 0x12} },       // set divide ratio
    { .length = 2, .cmd = { SSD1306_SETVCOMDETECT, 0x20} },    // vcom deselect to 0x20 // 0x40
    { .length = 2, .cmd = { SSD1306_CHARGEPUMP, 0x14} },       // Enable charge pump
    { .length = 1, .cmd = { SSD1306_DISPLAYALLON_RESUME} },
    { .length = 1, .cmd = { SSD1306_SET_DISPLAY | 1} },
};

static struct sd1306_cmd_t xconst sd1306_128_64_initcode[] = {
    { .length = 1, .cmd = { SSD1306_SET_DISPLAY | 0} }, // display off
    { .length = 2, .cmd = { SSD1306_MEMORYMODE, 0x0} }, // Page Addressing mode
    { .length = 1, .cmd = { SSD1306_SETSTARTLINE | 0x00} },    // First line to start scanning from
    { .length = 2, .cmd = { SSD1306_SETCONTRAST, 0x7F} },      // contast value to 0x7F according to datasheet
    { .length = 1, .cmd = { SSD1306_SEGREMAP | 0x01} },        // Use reverse mapping. 0x00 - is normal mapping
    { .length = 1, .cmd = { SSD1306_NORMALDISPLAY} },
    { .length = 2, .cmd = { SSD1306_SETMULTIPLEX, 63} },       // Reset to default MUX. See datasheet
    { .length = 2, .cmd = { SSD1306_SETDISPLAYOFFSET, 0x00} }, // no offset
    { .length = 2, .cmd = { SSD1306_SETDISPLAYCLOCKDIV, 0x80} },// set to default ratio/osc frequency
    { .length = 2, .cmd = { SSD1306_SETPRECHARGE, 0x22} },     // switch precharge to 0x22 // 0xF1
    { .length = 2, .cmd = { SSD1306_SETCOMPINS, 0x12} },       // set divide ratio
    { .length = 2, .cmd = { SSD1306_SETVCOMDETECT, 0x20} },    // vcom deselect to 0x20 // 0x40
    { .length = 2, .cmd = { SSD1306_CHARGEPUMP, 0x14} },       // Enable charge pump
    { .length = 1, .cmd = { SSD1306_DISPLAYALLON_RESUME } },
    { .length = 1, .cmd = { SSD1306_SET_DISPLAY | 1 } },
};

void sd1306_display_init(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;
    struct sd1306_cmd_t *initcode = NULL;
    int initcode_len = 0;
            
    if (!bus || !bus->ops)
        return;

    bus->ops->setup(bus);

    if (disp->height == 32) {
        initcode = sd1306_128_32_initcode;
        initcode_len = ARRAY_SIZE(sd1306_128_32_initcode);
    }
    else if (disp->height == 64) {
        initcode = sd1306_128_64_initcode;
        initcode_len = ARRAY_SIZE(sd1306_128_64_initcode);

    }
	
    for (int i = 0; i < initcode_len; ++i) {
		bus->ops->write(bus, 0, initcode[i].cmd, initcode[i].length);
    }
}



void sd1306_display_power_off(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;
    uint8_t cmd[] = { 0xAE };

    if (!bus || !bus->ops)
        return;

    bus->ops->write(bus, 0, &cmd[0], sizeof(cmd));
}


void sd1306_display_power_on(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;
    uint8_t cmd[] = { 0xAF };

    if (!bus || !bus->ops)
        return;
    
    bus->ops->write(bus, 0, &cmd[0], sizeof(cmd));
}

void sd1306_display_invert(struct display_t *disp, bool mode) {
    struct display_bus_t *bus = disp->bus;
    uint8_t cmd[] = { mode ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY };

    if (!bus || !bus->ops)
        return;
    
    bus->ops->write(bus, 0, &cmd[0], sizeof(cmd));
}

void sd1306_display_flip_horizontal(struct display_t *disp, bool mode) {
    struct display_bus_t *bus = disp->bus;
    uint8_t cmd[] = { SSD1306_SEGREMAP | (mode ? 0 : 1) };

    if (!bus || !bus->ops)
        return;
    
    bus->ops->write(bus, 0, &cmd[0], sizeof(cmd));
}

void sd1306_display_flip_vertical(struct display_t *disp, bool mode) {
    struct display_bus_t *bus = disp->bus;
    uint8_t cmd[] = { mode ? SSD1306_COMSCANINC : SSD1306_COMSCANDEC };

    if (!bus || !bus->ops)
        return;
    
    bus->ops->write(bus, 0, &cmd[0], sizeof(cmd));
}

void sd1306_display_clear_screen(struct display_t *disp) {

    screen_clear(disp->screen);

    disp->ops->draw_screen(disp);
}

void sd1306_display_set_brightness(struct display_t *disp, uint8_t value) {
    struct display_bus_t *bus = disp->bus;
    uint8_t cmd[] = { 0x81 };

    if (!bus || !bus->ops) {
        return;
	}

	bus->ops->write(bus, 0, &cmd[0], sizeof(cmd));
	bus->ops->write(bus, 0, &value, sizeof(value));
}

void sd1306_display_draw_screen(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;
    int cnt;
    uint8_t *pixels = screen_get_buffer(disp->screen);
    uint8_t cmds[] = { SSD1306_PAGEADDR, 0, 0xff, SSD1306_COLUMNADDR, 0, disp->width - 1 };
    
    if (!bus || !bus->ops)
        return;

    bus->ops->write(bus, 0x0, &cmds[0], sizeof(cmds) - 1);
    bus->ops->write(bus, 0x0, &cmds[sizeof(cmds) - 1], 1);

    cnt = (disp->height >> 3) * disp->width;
    bus->ops->write(bus, 0x40, &pixels[0], cnt);
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
