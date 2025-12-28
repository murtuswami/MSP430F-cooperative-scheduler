#include <msp430.h> 
#include "led.h"


void led_P1_init() {
    P1DIR |= BIT0;
}

void led_p1_on() {
    P1OUT |=  BIT0;
}

void led_p1_off() { 
    P1OUT &= ~BIT0;
}

bool led_status(){
    if ( (P1OUT & BIT0) == BIT0) {
        return true;
    }
    return false;
}
