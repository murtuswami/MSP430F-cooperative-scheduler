#include "cmd_set.h"
#include "command.h"
#include <string.h>
#include "pwm.h"
#include "uart.h"
#include <stdlib.h>


static const command_entry_t command_table[] =  {
    {"DUTY",set_duty_command}
};

static const uint8_t command_table_size = sizeof(command_table) / sizeof(command_table[0]);

void set_command(char tokens[][MAX_SC_LENGTH],uint16_t count){
      dispatch_command(tokens,count,command_table,command_table_size);
    
}
void set_duty_command(char tokens[][MAX_SC_LENGTH],uint16_t count){
    if( count == 0 ) {
        uart_puts("No duty cycle value provided\n");
        return;
    }
    
    float dc = atof(tokens[0]);  //todo use strof with proper error handling 
    set_pwm_duty_cycle(dc);
}

