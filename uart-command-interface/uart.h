#ifndef UART_H
#define UART_H

#include <msp430.h> 

void uart_init(void);
void uart_putc(char);
void uart_puts(const char*);

char * consume_command();

#endif
