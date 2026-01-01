/**
 * @file main.c
 * @brief PWM LED brightness controlled by ADC on MSP430.
 */


#include <msp430.h> 
#include <pwm.h>
#include <stdint.h>
#include <uart.h>
#include <adc.h>




void main (void)
{
    /* Stop the watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;   
    pin_init();                 /* Configure P1.2 as Timer_A PWM output */
    timer_init();               /* Configure Timer_A0 for PWM on CCR1 */
    set_duty_cycle(0.9f);       /* Start with 90% duty cycle (LED mostly on) */
    uart_init();                 /* Initialize UART for debug prints */
    adc_init();                  /* Initialize ADC on A0 with ISR + threshold */
    uart_puts("Initialized\n");
    __bis_SR_register(GIE);

    
    while(1) { 
        /* Enter LPM0; CPU sleeps until an interrupt (e.g. ADC) wakes it */
        _bis_SR_register(LPM0_bits | GIE);
        uint16_t adc_value; 
        /* Check if ADC module has published a new value */

        if(poll_adc_value(&adc_value)){
            /* Map 12‑bit value (0..4095) to duty cycle fraction (0.0..1.0) */
            float duty_cycle = (float) adc_value / 4095.0f ; 
            set_duty_cycle(duty_cycle);

            uart_put_uint16(adc_value);
            uart_puts("\n");
        }
    }
    

}
