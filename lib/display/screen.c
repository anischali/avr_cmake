#include "screen.h"


int monochrome_screen_get_pixel(struct screen_t *screen, struct point_t *p) {
    //int b_pos = (x * screen->width + y) / 8;

    return screen->pixels[0];
}


uint8_t * monochrome_screen_get_buffer(struct screen_t *screen) {
    //int b_pos = (x * screen->width + y) / 8;

    return screen->pixels;
}