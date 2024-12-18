
#ifndef __SCREEN_H_
#define __SCREEN_H_
#include <stdint.h>
#include "common.h"


struct screen_t;
struct point_t {
    int x;
    int y;
};

struct ebitmap_t {
    int width;
    int height;
    int bits_per_pixel;
    uint8_t *pixels;
};

struct screen_ops_t {
    uint32_t (*get_pixel)(struct screen_t *screen, struct point_t *p);
    void (*set_pixel)(struct screen_t *screen, struct point_t *p, uint32_t pixel);
    void (*fill)(struct screen_t *screen, uint32_t pixel);
    void (*draw_bitmap)(struct screen_t *screen, struct ebitmap_t *bitmap, struct point_t *offset);
};

struct screen_t {
    int width;
    int height;
    uint8_t *pixels;
    struct screen_ops_t *ops;
};

uint32_t monochrome_screen_get_pixel(struct screen_t *screen, struct point_t *p);
void monochrome_screen_set_pixel(struct screen_t *screen, struct point_t *p, uint32_t pixel);
void monochrome_screen_fill(struct screen_t *screen, uint32_t pixel);
void monochrome_screen_draw_bitmap(struct screen_t *screen, struct ebitmap_t *bitmap, struct point_t *offset);

uint32_t bitmap_get_pixel(struct ebitmap_t *bitmap, struct point_t *offset);
void bitmap_set_pixel(struct ebitmap_t *bitmap, struct point_t *offset, uint32_t pix);

#define DEFINE_MONOCHROME_SCREEN(name , _width, _height) \
    static struct screen_ops_t name##_ops = { \
        .get_pixel = monochrome_screen_get_pixel, \
        .set_pixel = monochrome_screen_set_pixel, \
        .fill = monochrome_screen_fill, \
        .draw_bitmap = monochrome_screen_draw_bitmap, \
    }; \
    static uint8_t name##_pixels_buf[_width * (_height >> 3)]; \
    static struct screen_t name = { \
        .width = _width, \
        .height = _height, \
        .pixels = name##_pixels_buf, \
        .ops =  &name##_ops, \
    }


uint8_t *screen_get_buffer(struct screen_t *screen);
uint32_t screen_get_pixel(struct screen_t *screen, struct point_t *p);
void screen_set_pixel(struct screen_t *screen, struct point_t *p, uint32_t pixel);
void screen_fill(struct screen_t *screen, int pixel);
void screen_clear(struct screen_t *screen);
void screen_draw_bitmap(struct screen_t *screen, struct ebitmap_t *bitmap, struct point_t *offset);
void screen_console_display(struct screen_t *screen);

#endif