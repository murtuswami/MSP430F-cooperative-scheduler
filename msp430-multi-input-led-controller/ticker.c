
#include "ticker.h"
#include <msp430.h>

/**
 * @file timer.c
 * @brief ACLK-based periodic tick using Timer1_A CCR0.
 *
 * Generates a fixed-period software tick (default: 5 ms) 
 * ACLK using TimerA1 in up mode. The tick is exposed via a simple
 * consume_tick() API that returns true once per timer period.
 *
 */

#define ACLK_FREQ_HZ 32768UL
#define TIMER_ID_DIV 1UL
#define TIMER_EX_DIV 1UL
#define TIMER_TOTAL_DIV (TIMER_ID_DIV * TIMER_EX_DIV)

/*
 * Compute CCR0 value for a given period in milliseconds.
 * Use 32-bit math to avoid overflow, then cast back to uint16_t.
 *
 * CCR0 = (ACLK_Hz * period_ms / (1000 * total_divider)) - 1
 */
#define TIMER_CCR0_FROM_MS(ms) ((uint16_t)( ( (uint32_t) (ACLK_FREQ_HZ) * (uint32_t) (ms)) / (1000UL * TIMER_TOTAL_DIV) - 1UL))

#define TIMER_PERIOD_MS 5UL /* 5 ms */

#define TIMER_CCR0_VALUE TIMER_CCR0_FROM_MS(TIMER_PERIOD_MS)

static volatile bool tick_flag = false;

/**
 * Configure Timer1_A to generate a periodic interrupt on CCR0.
 * The timer is left stopped; call ticker_on() to start it.
 */
void ticker_init() {

    TA1CTL |= TASSEL_1;                  /* Select ACLK as clock source */
    TA1CTL = (TA1CTL & ~ID_3) | ID_0;    /* Set ID to divide by 1, leave other bits unchanged */
    TA1EX0 = 0;                          /* Expansion divider /1 */
    TA1CTL |= TACLR;                     /* Clear timer and divider logic */
    TA1CCTL0 |= CCIE;                    /* Enable CCR0 interrupt */
    TA1CCR0 = TIMER_CCR0_VALUE;          /* Set CCR0 for desired tick period */
}
void ticker_on() {
    TA1CTL |= MC__UP;
}

bool consume_tick() { 
       // enter_lpm here and wakeup from ticker ISR 
    if(tick_flag) { 
        tick_flag = false;
        return true;
    }
    return false;

}

/*
 * Timer1_A CCR0 ISR.
 *
 * Note: For CCR0, the CCIFG flag is automatically cleared when the CCR0
 *       interrupt vector is serviced (per User's Guide). This is a special
 *       case; do not assume auto-clear for other CCRn/overflow flags.
 */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void timerA1Elapsed() {
    tick_flag = true; 
}


