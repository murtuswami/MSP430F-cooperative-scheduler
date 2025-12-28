#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>


void timer_on();
void timer_init();
bool consume_tick();

#endif
