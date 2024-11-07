#ifndef __FONT_H__
#define __FONT_H__
#include <stdint.h>
#include "display.h"
#include "screen.h"


struct font_t;

struct font_ops_t {
    void (*fill_pixels)(struct screen_t *screen, struct font_t *font, struct point_t *offset, const char c);
};

struct font_t {
    int width;
    int height;
    int pix_width;
    int pix_height;
    int bits_per_pixel;    
    uint8_t **pixels;
    struct font_ops_t *ops;
};

#define DECLARE_FONT(_p_width, _p_height, _width, _height, _depth, _pixels, _ops) \
    struct font_t { \
        .pix_width = _p_width, \
        .pix_height = _p_height, \
        .width = _width, \
        .height = _height, \
        .pixels = _pixels, \
        .ops = _ops, \
    }




#endif