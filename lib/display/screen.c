#include "screen.h"
#include "common.h"
#include "console.h"
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
    int pix;
    struct point_t off;

    for (int y = 0; y < bitmap->height; ++y) {
        for (int x = 0; x < bitmap->width; ++x) {
            off.x = x;
            off.y = y;
            pix = bitmap_get_pixel(bitmap, &off);
            off.x = __min(off.x + offset->x, screen->width); 
            off.y = __min(off.y + offset->y, screen->height); 
            screen_set_pixel(screen, &off, pix);
        }
    }

}

uint8_t * screen_get_buffer(struct screen_t *screen) {
    return screen->pixels;
}


void screen_console_display(struct screen_t *screen) {
    int pix;
    struct point_t p;

    for (int y = 0; y < screen->height; ++y) {
        usart_printf("\n\r");
        for (int x = 0; x < screen->width; ++x) {
            p.x = x;
            p.y = y;
            pix = screen_get_pixel(screen, &p);
            usart_printf("%c", pix > 0 ? '*' : '$');
        }
    }
    usart_printf("\n\r");
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


int bitmap_get_pixel(struct ebitmap_t *bitmap, struct point_t *offset) {

    switch (bitmap->bits_per_pixel)
    {
    case 1:
        uint8_t p8 = bitmap->pixels[(int)offset->y * (int)(bitmap->width / 8) + (int)(offset->x / 8)];
        int b_pos = offset->x % 8;
        return (p8 & BIT(b_pos));

    case 8:
        uint8_t pix8 = bitmap->pixels[(int)offset->y * bitmap->width + offset->x];
        return (int)pix8;

    case 16:
        uint16_t pix16;
        memcpy((uint8_t *)&pix16, &bitmap->pixels[offset->y * bitmap->width + offset->x], sizeof(pix16));
        return (int)pix16;

    case 32:
        uint32_t pix32;
        memcpy((uint8_t *)&pix32, &bitmap->pixels[offset->y * bitmap->width + offset->x], sizeof(pix32));
        return (int)pix32;

    default:
        return 0;
    }
}