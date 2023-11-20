#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FOSC 1843200
#define BAUD 9600
#define UBRR FOSC / 16 / BAUD - 1


static inline void usart_transmit(unsigned int data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}


static inline void usart_dump(const char *data, int size)
{
    for (int i = 0; i < size; ++i)
    {
        usart_transmit(data[i]);
    }
}


static inline void init_usart(void)
{
    UBRR0L = (unsigned char)UBRR_VALUE;
    UBRR0H = (unsigned char)(UBRR_VALUE >> 8);

    UCSR0B |= (1 << RXCIE0) | (1 << TXEN0);
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}



int main() {

    DDRB |= _BV(5);
    init_usart();
    sei();
    while (1)
    {
        PORTB ^= _BV(5);
        _delay_ms(1000);
    }

    return 0;
}