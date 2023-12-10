#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "usart.h"
#include "adc.h"
#include "pio.h"

#define min(a, b) ((a < b) ? a : b)
#define max(a, b) ((a > b) ? a : b)

static volatile uint8_t duty = 120;

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


void pwm_usart_recv(char c) {

    switch (c)
    {
        case '+':
            ++duty;
            break;
        case '-':
            --duty;
            break;
        case '0':
            duty = 0;
            break;
        case '9':
            duty = 255;
            break;
        case 's':
            duty += 10;
            break;
        case 'h':
            duty += 5;
            break;
        case 'r':
            duty -= 5;
            break;
        case 'b':
            duty -= 10;
            break;
    }
}


void rf_send(char c)
{
    uint8_t v = 0;
    for (int i = 0; i < 8; ++i)
    {
        v =  c & (1 << i);
        if (v)
            PORTD |= _BV(5);
        else
            PORTD &= ~_BV(5);
    }
}


void rf_recv(char *c)
{
    *c = 0;
    uint8_t v = 0;
    for (int i = 0; i < 8; ++i)
    {
        v =  PORTD & _BV(6); 
        if (v)
            *c |= (1 << i);
        else
            *c &= ~(1 << i);
    }
}


void rf_write(const char *fmt, ...)
{
    static char buffer[64];
    va_list args;
    va_start(args, fmt);
    int size = sizeof(buffer);
    memset(buffer, 0x0, sizeof(buffer));
    size = vsnprintf(buffer, sizeof(buffer), fmt, args);

    for (int i = 0; i < size; ++i)
    {
        rf_send(buffer[i]);
    }
    va_end(args);
}


void rf_read() {
    char c = 0;
    while (1)
    {
        rf_recv(&c);
        if (c != 0)
            usart_printf("recv: %c\n\r", c);
    }
}

ISR(BADISR_vect)
{
    pio_set(B, 5);
    sei();
}

ISR(PCINT0_vect)
{
    pio_set(D, 5);
    usart_printf("PCINT0 Hello world\n\r");
    sei();
}

ISR(PCINT1_vect)
{
    pio_set(D, 5);
    usart_printf("PCINT1 Hello world\n\r");
    sei();
}

ISR(PCINT2_vect)
{
    pio_disable_irq(2, 22)
    uint8_t v = pio_get(D, 6);
    pio_set_value(D, 5, v);
    usart_printf("PCINT2 Hello world %d\n\r", v);
    pio_enable_irq(2, 22);
    sei();
}

int main() {
    uint16_t p, val = 0;
    usart_init(pwm_usart_recv);
    test_pwm();
    sei();

    adc_setup(ADC_AREF_PIN, ADC_PRESCALER_128);
    while (1)
    {
        p = adc_read_sync(ADC_CHANNEL_0_PC0);
        if (p != val)
        {
            val = p;
            usart_printf("Potontiometer value is: %d\n\r", val);
        }
        /*
        if (OCR2A != duty)
        {
            duty = max(120, min(duty, 149));
            OCR2A = OCR2B = duty;
        }*/

        //rf_read();
        
        //rf_write("hello world!\n\r");
    }

    return 0;
}