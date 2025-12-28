#include <msp430.h> 
#include "led.h"

void led_init(){
    P1DIR |= BIT0;      //Set P1.0 to output (LED)
    P1OUT &= ~BIT0;     // force LED off initially (active-high LED)
    P4DIR |= BIT7; 
    P4OUT &= ~BIT7;
}

void toggle_led_one() {
    P1OUT ^= BIT0;
}

void toggle_led_two() {
    P4OUT ^= BIT7;
}
