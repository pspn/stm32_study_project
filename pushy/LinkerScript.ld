/* Linker script for STM32L476RG (1 MB FLASH, 128 KB RAM) */

ENTRY(Reset_Handler)

MEMORY
{
  FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
  RAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Top of stack = end of RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  /* Vector table */
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

  /* Code + constants */
  .text :
  {
    *(.text*)
    *(.rodata*)
  } > FLASH

  /* Initialized data */
  .data : AT(ADDR(.text) + SIZEOF(.text))
  {
    __data_start__ = .;
      *(.data*)
    __data_end__   = .;
  } > RAM

  /* Zero-initialized data */
  .bss :
  {
    __bss_start__ = .;
      *(.bss*)
    __bss_end__   = .;
  } > RAM

  /* Force alignment for safety */
  . = ALIGN(8);
}

