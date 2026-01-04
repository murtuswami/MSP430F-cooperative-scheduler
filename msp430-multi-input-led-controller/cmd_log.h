#ifndef CMDLOG_H
#define CMDLOG_H

#include "command.h"
#include <stdint.h>

void log_command(char tokens[][MAX_SC_LENGTH],uint16_t count); 
void log_adc_command(char tokens[][MAX_SC_LENGTH],uint16_t count);


#endif
