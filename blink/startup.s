    .syntax unified
    .cpu cortex-m4
    .thumb

    .section .isr_vector,"a",%progbits
    .word   _estack
    .word   Reset_Handler
    .rept   240                 /* reserve entries for all exceptions/IRQs */
      .word Default_Handler
    .endr

    .text
    .global Reset_Handler
Reset_Handler:
    /* (ถ้าใช้ .data/.bss ต้อง copy/zero ที่นี่) */
    LDR   R0, =main
    BX    R0

    .global Default_Handler
Default_Handler:
    B Default_Handler

