#include "screen.h"
#include "common.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>



int monochrome_screen_get_pixel(struct screen_t *screen, struct point_t *p) {
    uint8_t pix = screen->pixels[(int)p->y / 8 * screen->width + p->x];

    int b_pos = p->y % 8;

    return (pix & BIT(b_pos));
}

void monochrome_screen_set_pixel(struct screen_t *screen, struct point_t *p, int pixel) {
    uint8_t pix = screen->pixels[(int)p->y / 8 * screen->width + p->x];

    int b_pos = p->y % 8;

    if (pixel) {
        pix |= BIT(b_pos);
    }
    else {
        pix &= ~BIT(b_pos);
    }

    screen->pixels[(int)p->y / 8 * screen->width + p->x] = pix;
}


void monochrome_screen_fill(struct screen_t *screen, int pixel) {
    int cnt = ((screen->height >> 3) * screen->width);
    uint8_t val = pixel ? 255 : 0;
    memset(screen->pixels, val, cnt);
}


uint8_t * screen_get_buffer(struct screen_t *screen) {
    return screen->pixels;
}

int screen_get_pixel(struct screen_t *screen, struct point_t *p) {
    if (!screen->ops || !screen->ops->get_pixel)
        return 0;

    return screen->ops->get_pixel(screen, p);
}


void screen_set_pixel(struct screen_t *screen, struct point_t *p, int pixel) {
    
    if (!screen->ops || !screen->ops->set_pixel)
        return;

    screen->ops->set_pixel(screen, p, pixel);
}


void screen_fill(struct screen_t *screen, int pixel) {

    if (!screen->ops || !screen->ops->fill)
        return;

    screen->ops->fill(screen, pixel);
}

void screen_clear(struct screen_t *screen) {

   if (!screen->ops || !screen->ops->fill)
        return;

    screen->ops->fill(screen, 0);
}

