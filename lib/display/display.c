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




void display_power_on(struct display_t *disp) {
    if (!disp || !disp->ops || !disp->ops->power_on)
        return;
    
    disp->ops->power_on(disp);
}


void display_set_brightness(struct display_t *disp, uint8_t value) {

    if (!disp || !disp->ops || !disp->ops->set_brightness)
        return;
    
    disp->ops->set_brightness(disp, value);
}