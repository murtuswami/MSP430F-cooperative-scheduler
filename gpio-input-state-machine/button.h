#ifndef BUTTON_H
#define BUTTON_H

#include "stdbool.h"

void button_init();

void button_debounce();

void update_button_state();

bool consume_short_press_event();

bool consume_long_press_event();

#endif
