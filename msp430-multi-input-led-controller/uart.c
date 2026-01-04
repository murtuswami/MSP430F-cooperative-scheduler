/**
 * @file uart.c
 * @brief Simple UART driver with double-buffered RX command input.
 *
 * Features:
 * - USCI A1 UART at 115200 baud on P4.4 (TX) / P4.5 (RX)
 * - Blocking TX helpers: uart_putc(), uart_puts()
 * - Line-based RX into a double buffer, consumed via consume_command()
 */

#include <msp430.h>
#include "uart.h"
#include "string.h"
#include <stdio.h>
#include <stdbool.h>


#define UART_BUFFER_SIZE 64 

/* Double buffers for received commands */
static char buffer_a[UART_BUFFER_SIZE];
static char buffer_b[UART_BUFFER_SIZE];

/* 
 * main_buffer: owned by main code after consume_command() returns it
 * isr_buffer : owned by RX ISR while building the next command
 */

static  char * main_buffer = buffer_a;
static  char * volatile isr_buffer = buffer_b;

static volatile int isr_index = 0;
static volatile bool command_ready = false;

/**
 * Initialize USCI A1 for UART operation at 115200 baud using SMCLK.
 * P4.4 is configured as TX, P4.5 as RX, and RX interrupt is enabled.
 */
void uart_init(){
  UCA1CTL1 |= UCSWRST;      /* Hold USCI A1 in reset while configuring */
  UCA1CTL1 |= UCSSEL__SMCLK;    

 /* Configure P4.4 / P4.5 for UART */
  P4SEL |= BIT4 + BIT5; 
  P4DIR |= BIT4;
   /* 1 MHz SMCLK, 115200 baud (per User's Guide) */ 
  UCA1BR0 = 9;                              
  UCA1BR1 = 0;                              
  UCA1MCTL = UCBRS_1 + UCBRF_0;
  
  /* Release from reset and enable RX interrupt */       
  UCA1CTL1 &= ~UCSWRST;
  UCA1IE |= UCRXIE; 
}

void uart_putc(char c){
  while (!(UCA1IFG&UCTXIFG));    /* wait for TX buffer to be ready */
  UCA1TXBUF = c;  
}

void uart_puts(const char* c) {
  while(*c != '\0'){ 
    uart_putc(*c);
    ++c;
  }
}

void uart_put_uint16(const uint16_t v){
  char buf[6];
  snprintf(buf, sizeof buf, "%d", v);
  uart_puts(buf);

}


/**
 * Return pointer to the latest complete command string, or NULL.
 *
 * The command is built in the RX ISR into isr_buffer. When a newline is
 * received, command_ready is set. This function atomically swaps the ISR
 * and main buffers so main can process the completed command while the ISR
 * starts filling the other buffer.
 */

char * consume_command(){ 
    if (!command_ready) {
        return NULL;   /* no command available */
    }
   else {
          /* Disable UART RX interrupt while swapping buffers */
          UCA1IE &= ~UCRXIE;

          char *tmp = main_buffer;
          main_buffer = isr_buffer;   /*external scope now owns the filled buffer */
          isr_buffer = tmp;          /* ISR writes into the other buffer */
          isr_index = 0;            
          command_ready = false;
          
          /* Re‑enable UART RX interrupt */    
          UCA1IE |= UCRXIE;
          
          return main_buffer;

   }
}

/**
 * USCI A1 RX ISR: accumulate characters into isr_buffer until newline.
 * When a line terminator is seen, null-terminate the string and set
 * command_ready. If the previous command has not been consumed yet,
 * additional input is dropped.
 */
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                            // Vector 0 - no interrupt
   /* no interrupt */
  case 2:  {                              // Vector 2 - RXIFG
  /* RXIFG: receive character */
    char c = UCA1RXBUF;
      if (command_ready) {
       /* Previous command not yet consumed; drop input or implement a queue */
        break;
    }
    if (c == '\n' || c == '\r') {
      isr_buffer[isr_index] = '\0';
      command_ready = true;
    }
    else if (isr_index < UART_BUFFER_SIZE - 1) {
          /* Store character and advance index, keeping one slot for '\0' */
        isr_buffer[isr_index++] = c; 
    }   
    break;
  }
  case 4:break;  /* TXIFG: not used */
  default: break;
  }
} 

