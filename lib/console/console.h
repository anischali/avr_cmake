#ifndef _CONSOLE_H_
#define _CONSOLE_H_
#include "usart.h"

#ifndef CONSOLE_BUFFER_LENGTH
#define CONSOLE_BUFFER_LENGTH 32
#endif
static char console_buffer[CONSOLE_BUFFER_LENGTH];
static void (*console_send_command)(const char *command);
static char *buf = console_buffer;

void console_usart_recv(char c) {
    static int i = 0;

    switch (c)
    {   
        case 0:
        case '\n':
        case '\r':
            buf[i] = '\0';
            if (i > 0 && console_send_command)
                console_send_command(buf);

            usart_printf("\n\r$ ");
            i = 0;    
            return;
        case 127:
        case 8:
            if (i <= 0)
                return;

            buf[i] = 0;
            --i;
            usart_printf("\b \b");
            break;
        case 27:
            return;
        
        default:
            if (i <= CONSOLE_BUFFER_LENGTH) {
                buf[i++] = c;
                i = (i % CONSOLE_BUFFER_LENGTH) ? i : 0;
                usart_printf("%c", c);
            }
    }
}


#define console_init(callback) \
    console_send_command = callback; \
    usart_init(console_usart_recv)

#endif