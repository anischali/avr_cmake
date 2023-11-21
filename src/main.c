#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "pio.h"


static inline void test_pwm()
{
    pio_enable(B, 3, OUTPUT);
    pio_enable(D, 3, OUTPUT);
    TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
    TCCR2B = _BV(CS22);
    OCR2A = 150;
    OCR2B = 0;
}

int main() {

    usart_init();
    sei();
    test_pwm();
    uint8_t cnt = 0;
    while (1)
    {
        _delay_ms(250);
        usart_printf("loop (cmake): %d\n\r", ++cnt);
    }

    return 0;
}