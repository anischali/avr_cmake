#ifndef __USART_H
#define __USART_H

#ifndef BAUD
#define BAUD 38400
#endif
#include <util/setbaud.h>

void usart_init(void (*recv_callback)(char c));
void usart_printf(const char *fmt, ...);
#endif