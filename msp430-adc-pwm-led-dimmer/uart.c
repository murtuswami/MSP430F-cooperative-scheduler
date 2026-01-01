/**
 * @file uart.c
 * @brief Simple USCI_A1 UART driver (115200 baud, SMCLK).
 */



#include <msp430.h>
#include "uart.h"
#include "string.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


#define UART_BUFFER_SIZE 64 

/* 
 * Double‑buffer storage for received data / command parsing.
 * - `isr_buffer` is filled by the UART RX ISR.
 * - `main_buffer` is used by main context for processing.
 */
 
static char buffer_a[UART_BUFFER_SIZE];
static char buffer_b[UART_BUFFER_SIZE];

static  char * main_buffer = buffer_a;
static  char * volatile isr_buffer = buffer_b;

static volatile int isr_index = 0;
static volatile bool command_ready = false;

/* Initialize USCI_A1 for 115200 baud, SMCLK = 1 MHz, P4.4/5 */
void uart_init(){
  UCA1CTL1 |= UCSWRST;              /* Put USCI in reset for configuration */
  UCA1CTL1 |= UCSSEL__SMCLK;        /* Use SMCLK as clock source */
  P4SEL |= BIT4 + BIT5;             /* Select peripheral function for P4.4, P4.5 */
  P4DIR |= BIT4;                    /* P4.4 (TX) as output */
  /* 1 MHz / 115200 baud settings (see device user’s guide) */
  UCA1BR0 = 9;                      /* Baud rate divider low byte */
  UCA1BR1 = 0;                      /* Baud rate divider high byte */
  UCA1MCTL = UCBRS_1 + UCBRF_0;     /* Modulation pattern */
  UCA1CTL1 &= ~UCSWRST;             /* Release USCI from reset */
}

/* Blocking transmit of one character */
void uart_putc(char c){
  while (!(UCA1IFG&UCTXIFG));        
  UCA1TXBUF = c;  
}
/* Blocking transmit of a null-terminated string */
void uart_puts(const char* c) {
  while(*c != '\0'){ 
    uart_putc(*c);
    ++c;
  }

}
/* Print a uint16_t */
void uart_put_uint16(const uint16_t v){
  char buf[6];
  snprintf(buf, sizeof buf, "%d", v);
  uart_puts(buf);

}


