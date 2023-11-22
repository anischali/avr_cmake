#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "adc.h"
#include "pio.h"

static uint8_t duty_cycle = 255;

static inline void test_pwm()
{
    pio_enable(B, 3, OUTPUT);
    pio_enable(D, 3, OUTPUT);
    TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
    TCCR2B = _BV(CS22);
}

#define set_duty_cycle(duty) OCR2A = duty; OCR2B = duty

int clamp(int value)
{
    return value / 4;
}

int main() {
    pio_enable(C, 0, INPUT);
    usart_init();
    adc_setup(0, 7);
    sei();
    test_pwm();
    while (1)
    {
        duty_cycle = (uint8_t)clamp(adc_read_sync(1));
        set_duty_cycle(duty_cycle);
        _delay_ms(250);
        usart_printf("potentiometer: %d\n\r", duty_cycle);
    }

    return 0;
}