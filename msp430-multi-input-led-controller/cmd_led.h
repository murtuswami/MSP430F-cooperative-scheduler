#ifndef CMDLED_H
#define CMDLED_H

#include "command.h"
#include "stdint.h"

void led_command(char tokens[][MAX_SC_LENGTH],uint16_t count);
void led_p1_command(char tokens[][MAX_SC_LENGTH],uint16_t count);
void led_p4_command(char tokens[][MAX_SC_LENGTH],uint16_t count);

#endif
