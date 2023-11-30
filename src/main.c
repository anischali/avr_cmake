#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "usart.h"
#include "adc.h"
#include "pio.h"
#include "VirtualWire.h"

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
        //v =  c & (1 << i);
        if (c == '0')
            PORTD |= _BV(5);
        else
            PORTD &= ~_BV(5);
        _delay_ms(1);
    }
}


void rf_recv(char *c)
{
    *c = 0;
    uint8_t v = 0;
    //for (int i = 0; i < 8; ++i)
    //{
        v =  PIND & _BV(6);
        if (v)
            *c = '1';//*c |= (1 << i);
        else
            *c = '0';//*c &= ~(1 << i);
        //_delay_ms(1);
    //}
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


void rf_read(uint8_t length) {
    char c = 0;
    uint8_t count = 0;
    while (length--)
    {
        rf_recv(&c);
        if (c == '1')
        {
            count++;
        }
        else {
            count = 0;
        }
        if (c != 0 && count < 10)
            usart_printf("%c", c);
    }
}

ISR(BADISR_vect)
{
    sei();
}

ISR(PCINT0_vect)
{
    usart_printf("PCINT0 Hello world\n\r");
    sei();
}

ISR(PCINT1_vect)
{
    usart_printf("PCINT1 Hello world\n\r");
    sei();
}
volatile char c = 0;

ISR(PCINT2_vect)
{
    pio_disable_irq(2, 22)
    uint8_t v = pio_get(D, 6);
    
    static int i = 0;
    if (v)
        c |= (1 << i);
    else
        c &= ~(1 << i);
    ++i;
    if (i == 8 && c != 0)
    {
        usart_printf("%c", c);
        c = 0;
    }

    i = (i % 8);
    //usart_printf("PCINT2 Hello world %d\n\r", v);
    pio_enable_irq(2, 22);
    sei();
}

static inline void rf_receiver_setup(bool recv)
{
    vw_set_ptt_inverted(true); // Required for DR3100
	vw_setup(2000);	 // Bits per sec
    if (recv)
        vw_rx_start();
}

bool receiver = false;

int main() {
    usart_init(pwm_usart_recv);
    pio_enable(D, 5, OUTPUT);
    pio_enable(D, 6, INPUT);
    //pio_interrupt(2, 22);
    sei();
   // test_pwm();
    rf_receiver_setup(receiver);

    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    while (1)
    {
        if (receiver)
        {
            if (vw_get_message(buf, &buflen)) // Non-blocking
            {   
                usart_printf("%s\n\r", buf);
            }
        }
        else 
        {
            vw_send((uint8_t *)"hello world", 11);
            vw_wait_tx();
        }
    }
    return 0;
}