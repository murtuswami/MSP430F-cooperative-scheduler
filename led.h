#ifndef LED_H
#define LED_H

#include <stdbool.h>

void led_init();
void led_p1_toggle();
void led_p4_toggle();
void led_p1_on();
void led_p1_off();
void led_p4_on();
void led_p4_off();
bool led_p1_status();
bool led_p4_status();

#endif
