#ifndef __COMMON_AVR_H_
#define __COMMON_AVR_H_

#define __IO volatile
#define BIT(x) (1 << x)


static inline void __delay_ms(long ms) {
    long delay = ((ms * 1000) / (long)F_CPU);

    while (--delay > 0);
}

#endif