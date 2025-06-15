# stm32_study_project

A collection of STM32L476RG bare-metal examples demonstrating various techniques
for blinking the onboard LED, handling user button input via polling or interrupts,
and simple debouncing with timers.

## Board

- STM32 Nucleo-64 development board (STM32L476RG MCU)

## Table of Contents

- [Prerequisites](#prerequisites)
- [Examples](#examples)
  - [blink](#blink)
  - [blink-v2](#blink-v2)
  - [blink-exit](#blink-exit)
  - [pushy](#pushy)
  - [pushy-exti](#pushy-exti)
- [Building and Flashing](#building-and-flashing)
- [License](#license)

## Prerequisites

- ARM GCC toolchain (`arm-none-eabi-gcc`, `arm-none-eabi-objcopy`, etc.)
- [stlink-tools](https://github.com/stlink-org/stlink) (`st-flash`) for flashing

## Examples

Each example is located in its own subdirectory. Change into the directory and run:

```bash
make        # build the binary
make flash  # (optional) flash the binary to the board
```

### blink

Direct register access (no CMSIS) to blink the onboard LED (PA5) using a software
delay loop.

### blink-v2

Corrected register addresses and cleaned-up code compared to `blink`.

### blink-exit

CMSIS-based example using the external interrupt (EXTI) on the user button (PC13)
to toggle the LED.

### pushy

Simple polling example: reads the user button (PC13) and toggles the LED (PA5)
on each press with basic software debouncing.

### pushy-exti

Advanced EXTI-based example with timer (TIM2) for debouncing, NVIC configuration,
and low-power wait-for-interrupt (`WFI`) in the main loop.

## Building and Flashing

Build and flash any example by running `make` and `make flash`. Depending on your
platform, you may need `sudo` for `st-flash`.

```bash
cd <example-dir>
make
make flash
```

## License

This project is licensed under the Apache License 2.0. See the
[LICENSE](LICENSE) file for details.