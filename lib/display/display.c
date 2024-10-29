#include "display.h"



void display_init(struct display_t *disp) {
    if (!disp || !disp->ops || !disp->ops->init)
        return;
    
    disp->ops->init(disp);
}



void display_power_off(struct display_t *disp) {
    if (!disp || !disp->ops || !disp->ops->power_off)
        return;
    
    disp->ops->power_off(disp);
}

void display_reset(struct display_t *disp) {
    if (!disp || !disp->ops || !disp->ops->reset)
        return;
    
    disp->ops->reset(disp);
}


void display_power_on(struct display_t *disp) {
    if (!disp || !disp->ops || !disp->ops->power_on)
        return;
    
    disp->ops->power_on(disp);
}


void display_clear(struct display_t *disp) {
    if (!disp || !disp->ops || !disp->ops->clear)
        return;
    
    disp->ops->clear(disp);
}


void display_invert(struct display_t *disp, bool mode) {
    if (!disp || !disp->ops || !disp->ops->invert)
        return;
    
    disp->ops->invert(disp, mode);
}

void display_flip_horizontal(struct display_t *disp, bool mode) {
    if (!disp || !disp->ops || !disp->ops->flip_horizontal)
        return;
    
    disp->ops->flip_horizontal(disp, mode);
}

void display_flip_vertical(struct display_t *disp, bool mode) {
    if (!disp || !disp->ops || !disp->ops->flip_vertical)
        return;
    
    disp->ops->flip_vertical(disp, mode);
}


void display_set_brightness(struct display_t *disp, uint8_t value) {

    if (!disp || !disp->ops || !disp->ops->set_brightness)
        return;
    
    disp->ops->set_brightness(disp, value);
}

void display_draw_screen(struct display_t *disp) {
    if (!disp || !disp->ops || !disp->ops->draw_screen)
        return;
    
    disp->ops->draw_screen(disp);
}