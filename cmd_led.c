#include "cmd_led.h"
#include "command.h"
#include "string.h"
#include "led.h"
#include "uart.h"

static const command_entry_t command_table[] =  {
    {"P1",led_p1_command},
    {"P4",led_p4_command}
};


static const uint8_t command_table_size = sizeof(command_table) / sizeof(command_table[0]);

void led_command(char tokens[][MAX_SC_LENGTH],uint16_t count){
    dispatch_command(tokens ,count ,command_table,command_table_size);

}
void led_p1_command(char tokens[][MAX_SC_LENGTH],uint16_t count){
    if(count == 0 ) {
        uart_puts("Missing command\n");
        return;
    }
    if(strcmp(tokens[0],"ON") ==  0) {
        led_p1_on();
        return;
    };
    if(strcmp(tokens[0],"OFF") ==  0) {
        led_p1_off();
        return;
    };
    
    uart_puts("Unknown: ");
    uart_puts(tokens[0]);
    uart_putc('\n');

}
void led_p4_command(char tokens[][MAX_SC_LENGTH],uint16_t count){
    if(count == 0 ) {
        uart_puts("Missing command\n");
        return;
    }
    if(strcmp(tokens[0],"ON") ==  0) {
        led_p4_on();
        return;
    }
    if(strcmp(tokens[0],"OFF") ==  0) {
        led_p4_off();
        return;
    }
    
    uart_puts("Unknown: ");
    uart_puts(tokens[0]);
    uart_putc('\n');

}

