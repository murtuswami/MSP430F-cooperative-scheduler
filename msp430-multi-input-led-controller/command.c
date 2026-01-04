
#include "command.h"
#include "uart.h"
#include "led.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "cmd_led.h"
#include "cmd_set.h"
#include "cmd_log.h"



static const command_entry_t command_table[] =  {
    {"LED", led_command},
    {"LOG",log_command},
    {"SET",set_command},
};


static const uint8_t command_table_size = sizeof(command_table) / sizeof(command_table[0]);



uint16_t tokenize(const char command[], char tokenized[][MAX_SC_LENGTH]) {

    uint16_t word_index = 0;
    uint16_t cs_index = 0; 

    while(command[cs_index]) {
        uint16_t sc_index = 0; // points to index at second dimension
        while(command[cs_index] && command[cs_index] != ' '  ) {
            if(sc_index >= MAX_SC_LENGTH || word_index >= MAX_COMMAND_ARGS){ 
                return 0; 
            }
            tokenized[word_index][sc_index] = command[cs_index];
            ++sc_index;
            ++cs_index;
        }
        ++word_index; // increment first dimension 
        if(command[cs_index]) {
            ++cs_index;  //cs_index = ' ', increment to next character
        }
    }
    return word_index;    // there is now some valid command in tokenized (could be completely empty )
}

void dispatch_command(
    char tokens[][MAX_SC_LENGTH],
    uint16_t count,
    const command_entry_t* table,
    uint8_t table_size
) {
    if (count == 0) {
        uart_puts("Missing command\n");
        return;
    }
    uart_puts(tokens[0]);
    for (uint16_t i = 0; i < table_size; ++i) {
        if (strcmp(tokens[0], table[i].name) == 0) {
            table[i].handler(tokens + 1, count - 1);
            return;
        }
    }
    
    uart_puts("Unknown: ");
    uart_puts(tokens[0]);
    uart_putc('\n');
}

void parse_command(char tokens[][MAX_SC_LENGTH],uint16_t count) {
    dispatch_command(tokens,count,command_table,command_table_size);
  }


