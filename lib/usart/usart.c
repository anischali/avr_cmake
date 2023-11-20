#include "usart.h"
#include <avr/io.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char buffer[128];

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


void usart_init()
{
#define BAUD 38400
#include <util/setbaud.h>
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
   
    UCSR0B |= (1 << RXCIE0) | (1 << TXEN0);
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}