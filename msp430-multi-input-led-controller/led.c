#include <msp430.h> 
#include "led.h"


void led_init(){
    P1DIR |= BIT0;      
    P1OUT &= ~BIT0;     
    P4DIR |= BIT7; 
    P4OUT &= ~BIT7;
}

void led_p1_toggle() {
    P1OUT ^= BIT0;
}

void led_p4_toggle() {
    P4OUT ^= BIT7;
}


void led_p1_on() {
    P1OUT |=  BIT0;
}

void led_p1_off() { 
    P1OUT &= ~BIT0;
}

void led_p4_on(){
    P4OUT |= BIT7;

}
void led_p4_off() {
    P4OUT &= ~BIT7;
}

bool led_p1_status(){
    if ( (P1OUT & BIT0) == BIT0) {
        return true;
    }
    return false;
}


bool led_p4_status(){
    if ( (P4OUT & BIT7) == BIT0) {
        return true;
    }
    return false;
}
