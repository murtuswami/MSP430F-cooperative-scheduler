#include "button.h"
#include <msp430.h>
#include <stdint.h>
#include "uart.h"

#define NUMBER_OF_DEBOUNCE_SAMPLES 5
#define HELD_TICKS 400


//debounce variables
static uint8_t last_button_raw_state;
static uint8_t button_raw_stable_count = 0;

//State variables 

typedef enum {
    RELEASED,
    PRESSED,
    HELD
} ButtonState;

static int held_consecutive_count = 0; 
static ButtonState button_state = RELEASED; 
static bool long_press_event = false;
static bool short_press_event = false;

//shared 
static bool debounce_pressed = false; // true when we the debouncer has determined that a debounce press has occured



void button_init() {
    P1DIR &= ~ BIT1;    //Set P1.1 to input (button)
    P1REN |= BIT1;      // set P1.1 resistor to be on activated
    P1OUT |= BIT1;      // set p1.1 to use pull up resistor 
    last_button_raw_state = P1IN & BIT1;

}

// This need to tell us if the button is pressed 
// So we can have a variable button debounced pressed , which tells us statically in file scope 
// if there was a debounce press. 
void button_debounce() {
    
    uint8_t debounce_sample = P1IN & BIT1;          // BIT0 = pressed, BIT1 = released

    if (debounce_sample == last_button_raw_state ) {
        if (button_raw_stable_count < NUMBER_OF_DEBOUNCE_SAMPLES){
        button_raw_stable_count += 1; // need to trim to max
        } 
        
    } else {
        button_raw_stable_count = 1; 
        last_button_raw_state = debounce_sample;
    }

    if (button_raw_stable_count >= NUMBER_OF_DEBOUNCE_SAMPLES) {
        debounce_pressed = (last_button_raw_state == 0); // low voltage when pressed
    }
    
} 

void update_button_state(uint16_t g_ticks) {
    static uint16_t l_ticks;
    if (debounce_pressed) { //pressed
        switch (button_state){
            case RELEASED:              //RELEASED ==> PRESSED //when its pressed, start counting ticks 
                button_state = PRESSED;
                l_ticks = g_ticks;
                held_consecutive_count = 1;
                break;
            case PRESSED:
                if(held_consecutive_count < HELD_TICKS) {    //PRESSED==> PRESSED
                    uint16_t passed_ticks = g_ticks - l_ticks;
                    button_state = PRESSED;
                    l_ticks = g_ticks;
                    held_consecutive_count += passed_ticks;
                } else {                          //PRESSED ==> HELD
                    long_press_event = true;
                    button_state = HELD;
                }    
                break;
            case HELD:      //HELD ==> HELD
                break;
        }

    } 
    else { //released
        switch (button_state){
        case RELEASED:                  //RELEASED ==> RELEASED 
            break;
        case PRESSED:
            button_state = RELEASED;    //RELEASED ==> RELEASED
            held_consecutive_count = 0;
            short_press_event = true;
            break;
        case HELD:                      //HELD ==> RELEASED
            button_state = RELEASED;
            held_consecutive_count= 0;
            break;
        }
    }
    
}

bool consume_short_press_event() {
    if (short_press_event) {
        short_press_event = false;
        return true;
    } else {
        return false;
    }
}

bool consume_long_press_event() {
      if (long_press_event) {
        long_press_event = false;
        return true;
    } else {
        return false;
    }    
}
