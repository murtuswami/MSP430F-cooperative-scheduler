# ADC Controlled PWM LED Dimmer

MSP430F5529 project showing:

- Potentiometer input via ADC12 on A0
- PWM LED brightness control using Timer A0
- UART output of raw ADC values
- Threshold-based ADC change detection with low-power sleep/wake

## Hardware

- MSP430F5529 LaunchPad
- LED on P1.2 (Timer A0 CCR1)
- Potentiometer on P6.0 (ADC12 A0)
- USB UART via USCI A1 (P4.4, P4.5)

## Behavior

- Timer A0 generates PWM on P1.2.
- ADC12 continuously samples A0.
- The ADC interrupt:
  - Publishes a new value only when the change exceeds a threshold.
  - Wakes the CPU from LPM0.
- Main loop:
  - Sleeps in LPM0.
  - On wake, calls `poll_adc_value`.
  - Maps the 12-bit ADC value to a 0.0–1.0 duty cycle.
  - Updates PWM and prints the raw value over UART.

## Files

- `main.c`  – init, low-power main loop, ADC-to-PWM logic
- `pwm.c/h` – Timer A0 PWM setup and `set_duty_cycle`
- `adc.c/h` – ADC12 setup, ISR, and `poll_adc_value`
- `uart.c/h` – USCI A1 UART init and simple print functions

## Dependencies

Uses TI device support files from CCS:

- `msp430f5529.h` – register and bit definitions  
- `lnk_msp430f5529.cmd` – default linker script