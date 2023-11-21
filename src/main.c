#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "pio.h"

int main() {

    enable_pio(B, 5, OUTPUT_DIR);
    usart_init();
    sei();
    uint8_t cnt = 0;
    while (1)
    {
        pin_toggle(B, 5);
        _delay_ms(250);
        usart_printf("loop (cmake): %d\n\r", ++cnt);
    }

    return 0;
}