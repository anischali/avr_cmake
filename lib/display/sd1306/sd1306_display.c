#include "display.h"
#include "console.h"

static uint8_t sd1306_initcode[] = {
    SSD1306_DISPLAYOFF, // display off
    SSD1306_MEMORYMODE, 0x00, // Page Addressing mode
    SSD1306_COMSCANDEC,             // Scan from 127 to 0 (Reverse scan)
    SSD1306_SETSTARTLINE | 0x00,    // First line to start scanning from
    SSD1306_SETCONTRAST, 0x7F,      // contast value to 0x7F according to datasheet
    SSD1306_SEGREMAP | 0x01,        // Use reverse mapping. 0x00 - is normal mapping
    SSD1306_NORMALDISPLAY,
    SSD1306_SETMULTIPLEX, 63,       // Reset to default MUX. See datasheet
    SSD1306_SETDISPLAYOFFSET, 0x00, // no offset
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,// set to default ratio/osc frequency
    SSD1306_SETPRECHARGE, 0x22,     // switch precharge to 0x22 // 0xF1
    SSD1306_SETCOMPINS, 0x12,       // set divide ratio
    SSD1306_SETVCOMDETECT, 0x20,    // vcom deselect to 0x20 // 0x40
    SSD1306_CHARGEPUMP, 0x14,       // Enable charge pump
    SSD1306_DISPLAYALLON_RESUME,
    SSD1306_DISPLAYON,
};

void sd1306_display_init(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;

    if (!bus || !bus->ops)
        return;
    
    bus->ops->setup(bus);
    
	for (int i = 0; i < sizeof(sd1306_initcode); ++i)
		bus->ops->write(bus, 0, &sd1306_initcode[i], 1);
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


void sd1306_display_clear(struct display_t *disp) {
    struct display_bus_t *bus = disp->bus;
    int i, cnt;
    uint8_t *ptr = disp->screen->get_buffer(disp->screen);
    uint8_t cmd[] = {0x0, SSD1306_COLUMNADDR, 0, disp->width, SSD1306_PAGEADDR, 0, (disp->height >> 3) };

    if (!bus || !bus->ops)
        return;

    for (i = 0; i < sizeof(cmd); ++i) {
        bus->ops->raw_write(bus, &cmd[i], 1);
    }

    cnt = (disp->height >> 3) * disp->width;
    memset(ptr, 0x0, cnt);
    for (i = 0; i < cnt; ++i) {
        bus->ops->raw_write(bus, &ptr[i], 1);
    }
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

void sd1306_display_draw_screen(struct display_t *disp, struct screen_t *screen) {
    struct display_bus_t *bus = disp->bus;
    uint8_t cmd[] = { SSD1306_COLUMNADDR, 0, disp->width - 1, 0, SSD1306_PAGEADDR, (disp->height << 3) - 1 };
    int i, j;
    uint8_t val = 0;
    struct point_t p;

    if (!bus || !bus->ops)
        return;
    
    for (i = 0; i < sizeof(cmd); ++i)
        bus->ops->write(bus, 0, &cmd[i], 1);
    
    for (i = 0; i < disp->width; ++i) {
        for (j = 0; j < disp->height; ++j) {
            p.x = i;
            p.y = j;
            val = screen->get_pixel(screen, &p);
            bus->ops->write(bus, 0, &val, sizeof(val));
        }
    }
}


struct display_ops_t sd1306_display_ops = {
    .init = sd1306_display_init,
    .power_off = sd1306_display_power_off,
    .power_on = sd1306_display_power_on,
	.set_brightness = sd1306_display_set_brightness,
    .clear = sd1306_display_clear,
    .invert = sd1306_display_invert,
    .flip_horizontal = sd1306_display_flip_horizontal,
    .flip_vertical = sd1306_display_flip_vertical,
    .draw_screen = sd1306_display_draw_screen,
};
