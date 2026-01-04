#ifndef CMDSET_H
#define CMDSET_H

#include "command.h"
#include <stdint.h>

void set_command(char tokens[][MAX_SC_LENGTH],uint16_t count);
void set_duty_command(char tokens[][MAX_SC_LENGTH],uint16_t count);




#endif
