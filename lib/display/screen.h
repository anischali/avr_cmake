
#ifndef __SCREEN_H_
#define __SCREEN_H_
#include <stdint.h>

struct point_t {
    int x;
    int y;
};

struct screen_t {
    int height;
    int width;
    uint8_t *pixels;
    int (*get_pixel)(struct screen_t *screen, struct point_t *p);
    int (*get_pixels)(struct screen_t *screen, struct point_t *begin, struct point_t *end);
    uint8_t * (*get_buffer)(struct screen_t *screen);
};


int monochrome_screen_get_pixel(struct screen_t *screen, struct point_t *p);
uint8_t * monochrome_screen_get_buffer(struct screen_t *screen);

#define DEFINE_MONOCHROME_SCREEN(name, _height , _width) \
    static uint8_t name##_pixels_buf[(_height *  _width / 8)]; \
    static struct screen_t name = { \
        .height = _height, \
        .width = _width, \
        .pixels = name##_pixels_buf, \
        .get_pixel = monochrome_screen_get_pixel, \
        .get_buffer = monochrome_screen_get_buffer, \
    }

#endif