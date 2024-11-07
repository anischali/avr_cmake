#ifndef __FONT_H__
#define __FONT_H__
#include <stdint.h>
#include "display.h"
#include "screen.h"


struct font_t;
extern const uint8_t ascii_5x8_font[][5];


struct font_ops_t {
    void (*fill_pixels)(struct screen_t *screen, const struct font_t *font, struct point_t *offset, const char c);
};

void ascii_5x8_fill_pixels(struct screen_t *screen, const struct font_t *font, struct point_t *offset, const char c);
void screen_write_font(struct screen_t *screen, const struct font_t *font, struct point_t *offset, const char *fmt, ...);

struct font_t {
    const char *name;
    int length;
    int width;
    int height;
    int bits_per_pixel;    
    const uint8_t **pixels;
    struct font_ops_t *ops;
};

#define DECLARE_FONT(_name, _width, _height, _bpp, _pixels, _ops) \
    const struct font_t _name = { \
        .name = #_name, \
        .width = _width, \
        .height = _height, \
        .pixels = (const uint8_t **)_pixels, \
        .bits_per_pixel = _bpp, \
        .ops = _ops, \
    }

#endif