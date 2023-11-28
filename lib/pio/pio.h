#ifndef __PIO_H_
#define __PIO_H_
#include <avr/io.h>

enum pio_dir {
    INPUT = 0x1,
    OUTPUT = 0x2,
    INPUT_PULLUP= 0x4
};




#define pio_enable(port, pin, dir) {\
    if (dir & OUTPUT) \
        DDR##port |= _BV(pin); \
    else {\
        DDR##port &= ~_BV(pin); \
        if (dir & INPUT_PULLUP) {\
            PORT##port |= _BV(pin); \
        } \
    } \
}


#define pio_disable(port, pin)          ((DDR##port &= ~(_BV(pin))); (PORT##port &= ~(_BV(pin))))


#define pio_set(port, pin)              (PORT##port |= (_BV(pin)))
#define pio_clear(port, pin)            (PORT##port &= ~(_BV(pin)))
#define pio_set_value(port, pin, v)     ((v > 0) ? pio_set(port, pin) : pio_clear(port, pin))
#define pio_toggle(port, pin)           (PORT##port ^= (_BV(pin)))
#define pio_get(port, pin)              ((PIN##port & _BV(pin)) > 0)

#define pio_interrupt(pc_group, num)    (PCICR |= (_BV(PCIE##pc_group))); (PCMSK##pc_group |= (_BV(PCINT##num)))

#define pio_disable_irq(pc_group, num)  (PCMSK##pc_group &= ~(_BV(PCINT##num)));
#define pio_enable_irq(pc_group, num)   (PCMSK##pc_group |= (_BV(PCINT##num)));


#endif