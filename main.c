#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"


int main() {

    DDRB |= _BV(5);
    usart_init();
    sei();
    uint8_t cnt = 0;
    while (1)
    {
        PORTB ^= _BV(5);
        _delay_ms(250);
        usart_printf("loop: %d\n\r", ++cnt);
    }

    return 0;
}