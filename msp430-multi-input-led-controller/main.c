
#include <ticker.h>
#include "scheduler.h"
#include <msp430.h>
#include "adc.h"
#include "button.h"
#include "pwm.h"
#include "uart.h"
#include "ticker.h"
#include "tasks.h"
#include "led.h"


#define MAX_TICK_PERIOD 32767


static  task_t tasks[] = {  
    {
        .fn = poll_button,
        .period_ticks = 1,
        .next_run = 0,
    },
    /*
     {
        .fn = poll_adc,
        .period_ticks = 5,
        .next_run = 0
    },*/
    
     {
        .fn = poll_uart_rx,
        .period_ticks = 5,
        .next_run = 0
    } 
};

#define NUM_TASKS ((uint8_t)(sizeof(tasks) / sizeof(tasks[0])))

void main() {
    WDTCTL = WDTPW | WDTHOLD;   
    adc_init();
    button_init();
    led_init();     //onboard leds
    pwm_init();
     uart_init();
    ticker_init();
    ticker_on();
    __bis_SR_register(GIE);  // Enable global interrupts

    
    
    uint16_t ticks = 0; //wraparound at 65535
    
    while(1) {
        //enter lpm here? 
        if(consume_tick()) {
            scheduler_run(tasks,NUM_TASKS,ticks);
            ++ticks; 
        }
    }
}
