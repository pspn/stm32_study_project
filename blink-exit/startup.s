    .syntax unified
    .cpu cortex-m4
    .thumb

    /*------------------------------------------------------------------------------
      Vector table in FLASH
    ------------------------------------------------------------------------------*/
    .section .isr_vector,"a",%progbits
    .word   _estack                 /*  0: Initial SP            */
    .word   Reset_Handler           /*  1: Reset                 */
    .word   NMI_Handler             /*  2: NMI                   */
    .word   HardFault_Handler       /*  3: HardFault             */
    .word   MemManage_Handler       /*  4: MemManage             */
    .word   BusFault_Handler        /*  5: BusFault              */
    .word   UsageFault_Handler      /*  6: UsageFault            */
    .word   0                       /*  7: Reserved              */
    .word   0                       /*  8: Reserved              */
    .word   0                       /*  9: Reserved              */
    .word   0                       /* 10: Reserved              */
    .word   SVC_Handler             /* 11: SVCall                */
    .word   DebugMon_Handler        /* 12: DebugMonitor          */
    .word   0                       /* 13: Reserved              */
    .word   PendSV_Handler          /* 14: PendSV                */
    .word   SysTick_Handler         /* 15: SysTick               */

    /* External IRQs: fill IRQ0–IRQ39 with Default_Handler */
    /* That covers EXTI0…EXTI9 (6→10), EXTI9_5 (30), and up through IRQ39 */
    .rept 40
      .word Default_Handler
    .endr

    /* Now IRQ40 → EXTI15_10 */
    .word EXTI15_10_IRQHandler

    /*------------------------------------------------------------------------------
      Handlers
    ------------------------------------------------------------------------------*/
    .thumb_func
Reset_Handler:
    bl main
1:  b 1b

    .thumb_func
Default_Handler:
    b .

    /*------------------------------------------------------------------------------
      Weak alias declarations so all handlers resolve
    ------------------------------------------------------------------------------*/
    .weak NMI_Handler           ; .thumb_set NMI_Handler, Default_Handler
    .weak HardFault_Handler     ; .thumb_set HardFault_Handler, Default_Handler
    .weak MemManage_Handler     ; .thumb_set MemManage_Handler, Default_Handler
    .weak BusFault_Handler      ; .thumb_set BusFault_Handler, Default_Handler
    .weak UsageFault_Handler    ; .thumb_set UsageFault_Handler, Default_Handler
    .weak SVC_Handler           ; .thumb_set SVC_Handler, Default_Handler
    .weak DebugMon_Handler      ; .thumb_set DebugMon_Handler, Default_Handler
    .weak PendSV_Handler        ; .thumb_set PendSV_Handler, Default_Handler
    .weak SysTick_Handler       ; .thumb_set SysTick_Handler, Default_Handler

