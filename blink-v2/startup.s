    .syntax unified
    .cpu cortex-m4
    .thumb

    .section .isr_vector, "a"
    .word   _estack                 /* Initial stack pointer      */
    .word   Reset_Handler          /* Reset handler              */
    .word   NMI_Handler            /* NMI                        */
    .word   HardFault_Handler      /* HardFault                  */
    .word   MemManage_Handler      /* MemManage                  */
    .word   BusFault_Handler       /* BusFault                   */
    .word   UsageFault_Handler     /* UsageFault                 */
    .word   0                       /* Reserved                   */
    .word   0                       /* Reserved                   */
    .word   0                       /* Reserved                   */
    .word   0                       /* Reserved                   */
    .word   SVC_Handler            /* SVCall                     */
    .word   DebugMon_Handler       /* DebugMonitor               */
    .word   0                       /* Reserved                   */
    .word   PendSV_Handler         /* PendSV                     */
    .word   SysTick_Handler        /* SysTick                    */

    /* Default interrupt handlers */
    .weak NMI_Handler          ;  .thumb_set NMI_Handler, Default_Handler
    .weak HardFault_Handler    ;  .thumb_set HardFault_Handler, Default_Handler
    .weak MemManage_Handler    ;  .thumb_set MemManage_Handler, Default_Handler
    .weak BusFault_Handler     ;  .thumb_set BusFault_Handler, Default_Handler
    .weak UsageFault_Handler   ;  .thumb_set UsageFault_Handler, Default_Handler
    .weak SVC_Handler          ;  .thumb_set SVC_Handler, Default_Handler
    .weak DebugMon_Handler     ;  .thumb_set DebugMon_Handler, Default_Handler
    .weak PendSV_Handler       ;  .thumb_set PendSV_Handler, Default_Handler
    .weak SysTick_Handler      ;  .thumb_set SysTick_Handler, Default_Handler

    .section .text
Reset_Handler:
    /* Stack pointer already set by hardware from vector table */
    /* Initialize .data and .bss if you had globals (omitted here) */
    bl main
1:  b 1b  /* hang if main ever returns */

Default_Handler:
    b .     /* infinite loop */

    .size Default_Handler, .-Default_Handler

