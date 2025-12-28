#include <msp430.h>
#include "uart.h"
#include "led.h"
#include "string.h"
#include <stdio.h>
//******************************************************************************
//!
//!   Simple UART project to test
//!   Implements uart_init(), uart_putc(), uart_put()
//******************************************************************************

#define UART_BUFFER_SIZE 64 

// understand which of these need to be volatile and why 

static char buffer_a[UART_BUFFER_SIZE];
static char buffer_b[UART_BUFFER_SIZE];

static  char * main_buffer = buffer_a;
static  char * volatile isr_buffer = buffer_b;

static volatile int isr_index = 0;
static volatile bool command_ready = false;

  
//Use UCA1 as it is connected to USB on development board
void uart_init(){
  UCA1CTL1 |= UCSWRST;
  UCA1CTL1 |= UCSSEL__SMCLK; 
  P4SEL |= BIT4 + BIT5; //use peripheral 
  P4DIR |= BIT4; //TX set as output 
  UCA1BR0 = 9;                              // 1MHz 115200 (see User's Guide)
  UCA1BR1 = 0;                              // 1MHz 115200
  UCA1MCTL = UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA1CTL1 &= ~UCSWRST;
  UCA1IE |= UCRXIE; 
}

void uart_putc(char c){
  while (!(UCA1IFG&UCTXIFG)); // Wait for tx buffer to be empty
  UCA1TXBUF = c;  // load char into c into TX vbuffer 
}

void uart_puts(const char* c) {
  while(*c != '\0'){ 
    uart_putc(*c);
    ++c;
  }

}

char * consume_command(){ 
    if (!command_ready) {
        return NULL;   // no command available
    }
   else {
        __disable_interrupt();
          char *tmp = main_buffer;
          main_buffer = isr_buffer; // main buffer now has the command that was ready in ISR buffer
          isr_buffer = tmp;         // Now ISR Buffer points to what was main buffer
          isr_index = 0;            // so we reset the isr_index
          command_ready = false;    // command ready = false after we have swapped the buffers
        __enable_interrupt();  
          return main_buffer;

   }
}
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                            // Vector 0 - no interrupt
  case 2:  {                              // Vector 2 - RXIFG
    char c = UCA1RXBUF;
      if (command_ready) {
        // command not yet consumed by main; drop input (or implement a queue)
        break;
    }
    if (c == '\n' || c == '\r') {
      isr_buffer[isr_index] = '\0';
      command_ready = true;
    }
    else if (isr_index < UART_BUFFER_SIZE - 1) {
        isr_buffer[isr_index++] = c; //Keep filling buffer until one spot is left
    }   
    break;
  }
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
} 

