# ADC-Controlled PWM LED Dimmer

MSP430F5529 project demonstrating:

- Potentiometer input on ADC12 A0
- PWM brightness control of an LED via Timer_A0
- UART debug output of raw ADC values
- Threshold-based ADC change detection with low-power sleep/wake

## Hardware

- MSP430F5529 LaunchPad
- LED driven from P1.2 (Timer_A0 CCR1 output)
- Potentiometer connected to A0 (P6.0 / ADC12 A0)
- USB‑UART interface (USCI_A1 on P4.4/P4.5)

## Behavior

- Timer_A0 generates PWM on P1.2 using ACLK:
  - Period set by `TIMER_CCR0_VALUE` (default: $\approx 102$ Hz at $32{,}768$ Hz ACLK).
- ADC12 continuously samples A0 (repeat single channel mode).
- The ADC ISR:
  - Compares each new sample to the last published value.
  - Only “publishes” when $|raw - last\_published| > \text{ADC\_CHANGE\_THRESHHOLD}$.
  - Wakes the CPU from LPM0 when a new value should be processed.
- Main loop:
  - Sleeps in LPM0.
  - On wake, calls `poll_adc_value`:
    - If a new value is available, it:
      - Maps $0 \dots 4095$ to a duty cycle $d \in [0.0, 1.0]$.
      - Updates the PWM duty cycle.
      - Prints the raw ADC value over UART.

## Key Concepts

- Register-level configuration of:
  - Timer_A0 for PWM on CCR1 (P1.2).
  - ADC12 in repeat-single-channel mode on A0 (P6.0).
  - USCI_A1 UART at 115200 baud (P4.4 TX, P4.5 RX).
- Threshold-based noise filtering for analog input:
  - Only treat changes larger than a set number of ADC counts as “meaningful.”
- Simple ISR-to-main communication pattern:
  - ISR stores a value and a flag.
  - Main polls with `poll_adc_value(&value)` and processes each update once.
- Low-power operation:
  - Main sleeps in LPM0 and is woken by the ADC ISR when work is needed.

## Files

| File      | Purpose                                      |
|-----------|----------------------------------------------|
| `main.c`  | Top-level initialization and main loop       |
| `pwm.c`   | Timer_A0 PWM setup and `set_duty_cycle` API  |
| `adc.c`   | ADC12 setup, ISR, and `poll_adc_value` API   |
| `uart.c`  | USCI_A1 UART initialization and print helpers|
| `pwm.h`   | PWM function declarations                     |
| `adc.h`   | ADC function declarations                     |
| `uart.h`  | UART function declarations                    |

## Dependencies

This project uses TI-provided device support files:

| File                 | Source                    | Purpose                          |
|----------------------|---------------------------|----------------------------------|
| `msp430f5529.h`      | TI (included with CCS)    | Register and bit definitions     |
| `lnk_msp430f5529.cmd`| TI (included with CCS)    | Default linker script            |

All peripheral configuration and application logic are implemented directly at the register level using the [MSP430x5xx Family User's Guide (SLAU208)](https://www.ti.com/lit/ug/slau208q/slau208q.pdf).