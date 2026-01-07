#include <stdint.h>
#include "pwm.h"
#include "uart.h"
#include "adc.h"
#include "button.h"
#include "led.h"
#include "command.h"


void poll_adc(uint16_t g_ticks) {
    uint16_t adc_value; 
    /* Check if ADC module has published a new value */
    if(poll_adc_value(&adc_value)){
        /* Map 12‑bit value (0..4095) to duty cycle fraction (0.0..1.0) */
        float duty_cycle = (float) adc_value / 4095.0f ; 
        set_pwm_duty_cycle(duty_cycle);
        //uart_put_uint16(adc_value);
        //uart_puts("\n");
        }
}

void poll_button(uint16_t g_ticks) {
    button_debounce();
    update_button_state(g_ticks);
    if(consume_short_press_event()) {
        led_p1_toggle();
    }
    if(consume_long_press_event()) {
        led_p4_toggle();
    }
}

void poll_uart_rx(uint16_t g_ticks) {
    char * command_buf = consume_command();
    if (command_buf) {
        char tokens[MAX_COMMAND_ARGS][MAX_SC_LENGTH]  = {0}; //all null terminated 
        uint16_t word_count = tokenize(command_buf, tokens);
        if(word_count > 0){
            parse_command(tokens,word_count);
        }
    }
}

