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
    int cnt = (screen->height >> 3) * screen->width;
    uint8_t val = pixel ? 255 : 0;
    memset(screen->pixels, val, cnt);
}

void monochrome_screen_draw_bitmap(struct screen_t *screen, struct ebitmap_t *bitmap, struct point_t *offset) {
    struct point_t lim = { 
        .x = __min(offset->x + bitmap->width, screen->width),
        .y = __min(offset->y + bitmap->height, screen->height),
    };

    for (int y = offset->y, p = 0; y < lim.y && p < bitmap->height; ++y, ++p) {
        memcpy(&screen->pixels[y / 8 * screen->width + offset->x], &bitmap->pixels_array[p], 1);
    }
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


void screen_draw_bitmap(struct screen_t *screen, struct ebitmap_t *bitmap, struct point_t *offset) {
    if (!screen->ops || !screen->ops->draw_bitmap)
        return;

    screen->ops->draw_bitmap(screen, bitmap, offset);
}