#ifndef __PIO_H_
#define __PIO_H_
#include <avr/io.h>

enum pio_dir {
    INPUT_DIR = 0x1,
    OUTPUT_DIR = 0x2,
    INPUT_PULLUP= 0x4
};

#define enable_pio(port, pin, dir)\
if (dir & OUTPUT_DIR) \
    DDR##port |= _BV(pin); \
else {\
    DDR##port &= ~_BV(pin); \
    if (dir & INPUT_PULLUP) {\
        PORT##port |= _BV(pin); \
    } \
}


#define disable_pio(port, pin)      DDR##port &= ~_BV(pin); PORT##port &= ~_BV(pin)


#define pin_set(port, pin)          PORT##port |= _BV(pin)
#define pin_clear(port, pin)        PORT##port &= ~_BV(pin)
#define pin_toggle(port, pin)       PORT##port ^= _BV(pin)
#define pin_get(port, pin)        (PORT##port & _BV(pin))

#endif