
/**
 * @file adc.c
 * @brief ADC12 A0 sampling with change-threshold publishing and ISR wake.
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>



#define ADC_CHANGE_THRESHHOLD 100        /* raw counts; 12-bit ADC */


static volatile bool published = false;         /* false => new value pending */
static volatile uint16_t publish_value = 0; 

/* Configure ADC12 to continuously sample A0 (P6.0) with MEM0 interrupt */
void adc_init() {

    /* Route P6.0 to ADC input A0 */
    P6SEL |= BIT0;

    /* Disable ADC to allow configuration changes */
    ADC12CTL0 &= ~ADC12ENC;

    /* Sample‑and‑hold and multiple sample mode */
    ADC12CTL0 &= ~ADC12MSC;           
    ADC12CTL0 |= ADC12SHT0_6;         /* 128 ADC clocks sample time on SHT0 */
    ADC12CTL0 |= ADC12MSC;            /* enable repeated conversions */

    /* Use external AVCC/AVSS reference (on‑chip ref off) */
    ADC12CTL0 &= ~ADC12REFON;

    /* Start address: ADC12MEM0 (CSTARTADD = 0) */
    ADC12CTL1 &= ~(ADC12CSTARTADD0 | ADC12CSTARTADD1 |
                   ADC12CSTARTADD2 | ADC12CSTARTADD3);

    /* Trigger source: software (SHS = 0), sampling timer (SHP = 1) */
    ADC12CTL1 &= ~(ADC12SHS0 | ADC12SHS1); /* software trigger */
    ADC12CTL1 |= ADC12SHP;                 /* use sampling timer */

    /* ADC clock: SMCLK /1 */
    ADC12CTL1 &= ~(ADC12DIV0 | ADC12DIV1 | ADC12DIV2); /* divider /1 */
    ADC12CTL1 |= (ADC12SSEL0 | ADC12SSEL1);            /* SMCLK source */

    /* Conversion mode: repeat‑single‑channel */
    ADC12CTL1 |= ADC12CONSEQ_2;

    /* ADC12CTL2: 12‑bit resolution, no additional pre‑divider */
    ADC12CTL2 &= ~ADC12PDIV;      /* pre‑divider /1 */
    ADC12CTL2 |= ADC12RES_2;      /* 12‑bit resolution */

    /* Enable interrupt for ADC12MEM0 (corresponds to channel A0 here) */
    ADC12IE |= ADC12IE0;

    /* Memory control for ADC12MEM0:
     * - EOS: end of sequence at MEM0 (only one channel).
     * - SREF: AVCC/AVSS.
     * - INCH: A0.
     */
    ADC12MCTL0 |= ADC12EOS;   /* end of sequence at MEM0 */
    ADC12MCTL0 &= ~(ADC12SREF0 | ADC12SREF1 | ADC12SREF2); /* AVCC/AVSS */
    ADC12MCTL0 &= ~(ADC12INCH0 | ADC12INCH1 | ADC12INCH2 | ADC12INCH3); /* A0 */

    /* Turn ADC on, enable conversions, and start conversion loop */
    ADC12CTL0 |= ADC12ON;
    ADC12CTL0 |= ADC12ENC;
    ADC12CTL0 |= ADC12SC;


}

/* Return true once per new value; copy into *external_value */
bool poll_adc_value(uint16_t *external_value){
    if(!published) {
         *external_value = publish_value;
        published = true;   /* mark value as consumed */
        return true; 
    }
    return false;
}

/* ADC12 MEM0 ISR: publish sample if change > threshold, wake from LPM0 */

#pragma vector=ADC12_VECTOR
__interrupt void ADC12_interrupt(void) {
    if (ADC12IV == ADC12IV_ADC12IFG0) { 
        static uint16_t last_published = 0;
        uint16_t raw  = ADC12MEM0; 
        uint16_t diff = (raw > last_published) ? (raw - last_published) : (last_published - raw);
        if(diff >  ADC_CHANGE_THRESHHOLD ) {
            last_published = raw; /* update reference point */
            publish_value = raw;  /* value to be consumed by main */
            published = false; /* mark as needing publication */
            
            /* Wake main from LPM0 so it can process the new value */
            __bic_SR_register_on_exit(LPM0_bits);  
        }
        /* Else: ignore small jitter; do not update or wake main. */
    }
    /* No other ADC12IV cases are expected: only MEM0 interrupt is enabled. */

}



