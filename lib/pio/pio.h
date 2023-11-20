#ifndef __PIO_H_
#define __PIO_H_
#include <avr/io.h>


#define enable_pio(port, pin)       DDR##port |= _BV(pin)
#define disable_pio(port, pin)      DDR##port &= ~_BV(pin)


#define pin_set(port, pin)          PORT##port |= _BV(pin)
#define pin_clear(port, pin)        PORT##port &= ~_BV(pin)
#define pin_toggle(port, pin)       PORT##port ^= _BV(pin)

#endif