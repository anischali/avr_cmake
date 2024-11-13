#ifndef __COMMON_AVR_H_
#define __COMMON_AVR_H_

#define __IO volatile
#define BIT(x) (1 << x)


static inline void delay_ms(long ms) {
    ms *= 1000 / F_CPU;
    while(--ms > 0);
}


#define __assert(cond, ret_code) \
    if (!(cond)) return ret_code;


#define __max(x, y) ((x > y) ? x : y)
#define __min(x, y) ((x < y) ? x : y)


#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#define xconst(v) __attribute__ ((used, section(".progmemx."#v))) v

#endif