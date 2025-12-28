
#include "driverlib.h"
#include "timer.h"
#include "button.h"
#include "led.h"

//******************************************************************************
//!
//!   Turn on/off LED P1.0 with a short press of button P1.1 or turn on/off LED P4.7 With a long hold of P1.1 
//!
//******************************************************************************


void main (void)
{   
    WDTCTL = WDTPW | WDTHOLD; // disable watchdog timer
    timer_init();
    button_init();
    led_init();
    __enable_interrupt();
    timer_on();
    
    while(1){

        // __bis_SR_register(LPM0_bits | GIE); //enter LMP0 until timer interrupt 

        if(consume_tick()) {
            button_debounce();
            update_button_state();
            if(consume_short_press_event()) {
                toggle_led_one();
            }
            if(consume_long_press_event()) {
                toggle_led_two();
            }
        }
    }
    
}

