
#include "command.h"
#include "uart.h"
#include "led.h"
#include <string.h>
#include <stdio.h>

static const command_entry_t command_table[] =  {
{"LED", led_command}
};

size_t command_table_size = sizeof(command_table) / sizeof(command_table[0]);


void parse_command(const char* c) {

  char first[25] = {'\0'};
  int i = 0; 
    
  while(*c != ' ' && *c != '\0') {
        if (i >= 24) { 
            uart_puts("First arguement cannot be greater than 25 characters \n");
            return;
        }
        first[i] = *c;
        ++c; 
        ++i; 
    }
    if(*c != '\0') {
        ++c;
    }
    first[i] = '\0';
    size_t j;
    for(j = 0 ; j < command_table_size; ++j) {
        if(strcmp(first, command_table[j].name) == 0 ){
            command_table[j].handler(c);
            return;
        }   
    }
  }


void led_command(const char* args){ 

    if(strcmp(args,"ON") ==  0) {
        led_p1_on();
    }
    else if(strcmp(args,"OFF") ==  0) {
        led_p1_off();
    }
    else if(strcmp(args,"STATUS") == 0 ) {
         if (led_status()) {
            uart_puts("LED P1.0 is ON. \n");
        }
        else {
            uart_puts("LED P1.0 is OFF. \n");
        }
    }
    else {
        uart_puts("LED");
        uart_puts(args);
        uart_putc(' ');
        uart_puts("Is not a recognized command \n");
    }
    
}

