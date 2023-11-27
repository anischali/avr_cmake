#ifndef __USART_H
#define __USART_H

void usart_init(void (*recv_callback)(char c));
void usart_printf(const char *fmt, ...);
#endif