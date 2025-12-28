# Embedded Portfolio

Bare-metal firmware projects on MSP430 and STM32 — drivers, bootloader, RTOS.

## About

Firmware projects demonstrating register-level embedded development.
All peripheral drivers written from scratch using reference manuals — no HAL abstractions.
Focus on understanding hardware at the lowest level.

## Projects

### MSP430

| Project | Description | Key Concepts |
|---------|-------------|--------------|
| [gpio-input-state-machine](./msp430/gpio-input-state-machine/) | Timer-driven button handler with debouncing and press/hold detection | GPIO, Timer, FSM, interrupts |
| [uart-command-interface](./msp430/uart-command-interface/) | Serial console with double-buffered RX and command dispatch table | UART, interrupts, double-buffer, function pointers |

### STM32

| Project | Description | Key Concepts |
|---------|-------------|--------------|
| Coming soon | | |

## Skills Demonstrated

- Register-level peripheral configuration (GPIO, UART, Timer, ADC, I2C, SPI)
- Interrupt handling and ISR design
- Finite state machines for input processing
- Double-buffer techniques for ISR/main communication
- Command parser architecture with function pointer tables

## Tools

| Category | Tools |
|----------|-------|
| MCUs | MSP430F5529, STM32F401RE |
| IDEs | Code Composer Studio, STM32CubeIDE |
| Languages | C |

## Contact

- Email: harsharamachandran14@gmail.com