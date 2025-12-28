# UART Command Interface

Interrupt-driven serial console on MSP430F5529 with double-buffered 
command reception and extensible command dispatch table.

## Hardware

- MSP430F5529 LaunchPad
- USB backchannel UART (UCA1)
- Onboard LED (P1.0)

## Usage

Connect via serial terminal (115200 8N1):
LED ON -> Turn on LED
LED OFF -> Turn off LED
LED STATUS -> Print LED state



## Key Concepts

| Concept | Implementation |
|---------|----------------|
| UART RX | Interrupt-driven, non-blocking |
| Buffer management | Double-buffer swap between ISR and main |
| Command dispatch | Function pointer table, easily extensible |
| Data handoff | `consume_command()` pattern with interrupt-safe swap |

## Files

| File | Purpose |
|------|---------|
| `uart.c/h` | UART driver, ISR, buffer management |
| `command.c/h` | Parser and command table |
| `led.c/h` | LED control |

---

## Dependencies

This project uses TI-provided files:

| File | Source | Purpose |
|------|--------|---------|
| `msp430f5529.h` | TI (included with CCS) | Register and bit definitions |
| `lnk_msp430f5529.cmd` | TI (included with CCS) | Default linker script |

All peripheral configuration and application logic is register-level code written from scratch using the [MSP430x5xx Family User's Guide (SLAU208)](https://www.ti.com/lit/ug/slau208q/slau208q.pdf).