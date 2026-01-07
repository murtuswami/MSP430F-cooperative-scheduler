/**
 * @file pwm.c
 * @brief PWM on P1.2 using Timer_A0 CCR1 (ACLK).
 */
 
#include <msp430.h>
#include <stdint.h>
#include <pwm.h>
#define TIMER_CCR0_VALUE ((uint16_t)320)


/* Configure P1.2 as Timer_A0 CCR1 output */



/* Initialize Timer_A0 for PWM on CCR1 */
void pwm_init(void)
{   
     P1DIR |= BIT2;  /* P1.2 as output */
    P1SEL |= BIT2;  /* P1.2 function select: TA0.1 (Timer_A CCR1 output) */

    TA0CTL = TASSEL_1 | MC_0;           /* ACLK, stop */
    TA0CTL = (TA0CTL & ~ID_3) | ID_0;  /* /1 divider */
    TA0EX0 = 0;                        
    TA0CTL |= TACLR;                   /* Clear timer to start from 0 */

    TA0CCR0 = TIMER_CCR0_VALUE;        

    TA0CTL &= ~TAIE;                   /* Disable Timer_A overflow interrupt */

    TA0CCTL1 &= ~CAP;                  /* Compare mode (not capture) */
    TA0CCTL1 = (TA0CCTL1 & ~OUTMOD_7) | OUTMOD_7; /* reset/set mode */

    TA0CCTL1 &= ~CCIE;                 /* no CCR1 IRQ */

    TA0CCR1 = 0;                      /* 0% duty */

    TA0CTL |= MC__UP;                  /* up mode */
}

/* Set PWM duty cycle in [0.0, 1.0] */
void set_pwm_duty_cycle(const float duty_cycle)
{
    float d = duty_cycle;

    if (d < 0.0f) d = 0.0f;
    if (d > 1.0f) d = 1.0f;

    TA0CCR1 = (uint16_t)(TIMER_CCR0_VALUE * d);
}            



