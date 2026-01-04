#ifndef TICKER_H
#define TICKER_H

#include <stdint.h>
#include <stdbool.h>


void ticker_on();
void ticker_init();
bool consume_tick();

#endif
