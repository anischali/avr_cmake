#ifndef __COMMON_AVR_H_
#define __COMMON_AVR_H_

#define __IO volatile
#define BIT(x) (1 << x)


static inline void delay_ms(long ms) {
    ms *= 1000 / F_CPU;
    while(--ms > 0);
}

#endif