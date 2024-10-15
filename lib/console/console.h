#ifndef _CONSOLE_H_
#define _CONSOLE_H_
#include "usart.h"


void console_init(void (*callback)(char *buf));
#endif