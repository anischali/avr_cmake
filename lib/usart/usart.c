#include "usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char buffer[128];

static void (*usart_recv_callback)(char c) = NULL;

static inline void usart_transmit(unsigned int data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}


void usart_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int size = 128;
    memset(buffer, 0x0, sizeof(buffer));
    size = vsnprintf(buffer, sizeof(buffer), fmt, args);

    for (int i = 0; i < size; ++i)
    {
        usart_transmit(buffer[i]);
    }
}


void usart_init(void (*recv_callback)(char c))
{
#ifndef BAUD
#define BAUD 38400
#endif
#include <util/setbaud.h>
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
   
    UCSR0B |= _BV(TXEN0) | _BV(RXEN0);
    
    if (recv_callback) {
        UCSR0B |= _BV(RXCIE0);
        usart_recv_callback = recv_callback;
    }

    UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}


ISR(USART_RX_vect) {
    char c = UDR0;
    if (usart_recv_callback)
    {
        usart_recv_callback(c);
    }
    sei();
}