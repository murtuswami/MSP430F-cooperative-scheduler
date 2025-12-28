#include "uart.h"
#include "led.h"
#include "command.h"

void main (void)
{   
    WDTCTL = WDTPW + WDTHOLD;   
    uart_init();
    led_P1_init();
    __enable_interrupt();
    uart_puts("UART initiated \n");
    while(1){
        char * command_buf = consume_command();
        if (command_buf) {
            parse_command(command_buf);
        }
      }
}
    //__bis_SR_register(LPM0_bits | GIE);
    //__no_operation();
