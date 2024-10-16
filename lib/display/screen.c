#include "screen.h"


int monochrome_screen_get_pixel(struct screen_t *screen, struct point_t p) {
    //int b_pos = (x * screen->width + y) / 8;

    return screen->pixels[0];
}