#include "cmd_log.h"
#include "command.h"
#include "uart.h"


static const command_entry_t command_table[] =  {
    {"ADC",log_adc_command}
};


static const uint8_t command_table_size = sizeof(command_table) / sizeof(command_table[0]);

void log_command(char tokens[][MAX_SC_LENGTH],uint16_t count){
        dispatch_command(tokens,count,command_table,command_table_size);
   
}
void log_adc_command(char tokens[][MAX_SC_LENGTH],uint16_t count){
    //todo implement adc logging 
    if( count == 0 ) {
        uart_puts("ADC Logging not yet implemented\n");
        return;
    }

    uart_puts("Unknown: ");
    uart_puts(tokens[0]);
    uart_putc('\n');
}
