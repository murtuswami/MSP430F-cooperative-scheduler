# GPIO Input State Machine

Timer-driven button handler on MSP430F5529 demonstrating:

- 5ms tick-based polling using Timer_A
- Software debouncing with configurable sample count
- Finite state machine: RELEASED → PRESSED → HELD
- Event consumption pattern for short press / long press detection

## Hardware

- MSP430F5529 LaunchPad
- Onboard LEDs (P1.0, P4.7)
- Onboard button (P1.1)

## Key Concepts

- Register-level GPIO and Timer_A configuration
- State machine design for input handling
- Separation of debouncing logic from state logic


## Dependencies

This project uses TI-provided files:

| File | Source | Purpose |
|------|--------|---------|
| `msp430f5529.h` | TI (included with CCS) | Register and bit definitions |
| `lnk_msp430f5529.cmd` | TI (included with CCS) | Default linker script |

All peripheral configuration and application logic is register-level code written from scratch using the [MSP430x5xx Family User's Guide (SLAU208)](https://www.ti.com/lit/ug/slau208q/slau208q.pdf).