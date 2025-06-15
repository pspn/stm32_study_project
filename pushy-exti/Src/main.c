#include "main.h"

// Global variable to store LED state, volatile as it's modified by ISR
volatile uint8_t g_led_on = 0;

// Debounce delay in milliseconds (one-shot timer)
#define DEBOUNCE_DELAY_MS 50U

// This is a dummy function to satisfy the linker, which expects this
// when linking against parts of the C library. In a bare-metal context
// with -nostdlib, we provide an empty one.
void _init(void) {}

/**
  * @brief  EXTI line 10 to 15 interrupt handler.
  *         This function handles the interrupt for PC13 (User Button).
  */
void EXTI15_10_IRQHandler(void) {
    // Check if the interrupt was triggered by EXTI Line 13
    if (EXTI->PR1 & EXTI_PR1_PIF13) { // PR1 is for lines 0-31. PIF13 for line 13.

        // --- Simple Debounce Strategy ---
        // 1. Disable this specific EXTI line temporarily to prevent re-entry during debounce.
        EXTI->IMR1 &= ~EXTI_IMR1_IM13; // IMR1 for lines 0-31

        // 2. Toggle LED state
        g_led_on ^= 1;
        if (g_led_on) {
            GPIOA->BSRR = GPIO_BSRR_BS5; // Turn LED ON
        } else {
            GPIOA->BSRR = GPIO_BSRR_BR5; // Turn LED OFF
        }

        // 3. Clear the EXTI pending flag for line 13 by writing '1' to it.
        // This MUST be done, otherwise the ISR will be called again immediately.
        EXTI->PR1 = EXTI_PR1_PIF13;

        // 4. Start one-shot debounce timer (TIM2). EXTI will be re-enabled in timer ISR.
        TIM2->SR   &= ~TIM_SR_UIF;    // Clear update interrupt flag
        TIM2->CNT   = 0U;             // Reset counter
        TIM2->CR1  |= TIM_CR1_CEN;    // Start timer (one-pulse mode)
    } else {
        // If other EXTI lines (10-15) could trigger this ISR and are configured,
        // handle them or clear their pending flags here.
        // For this example, we only expect EXTI13.
    }
}

/**
 * @brief  TIM2 interrupt handler for debounce timing.
 *         Re-enables EXTI line 13 after debounce period.
 */
void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        EXTI->IMR1 |= EXTI_IMR1_IM13;
    }
}

int main(void) {
    // 1. Enable Clocks
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN;
    // Enable SYSCFG clock (for EXTI functionality). SYSCFG is on APB2 bus.
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // 2. Configure GPIO Pin PA5 as Output
    GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
    GPIOA->MODER |= GPIO_MODER_MODE5_0;

    // 3. Configure GPIO Pin PC13 as Input for the user button
    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk; // 00: Input mode
    // Optional: Configure pull-resistor for PC13 if needed (e.g., pull-down if button connects to VDD)
    // Most Nucleo boards have B1 (user button) on PC13, often active-high.
    // If it's active high and no external pull-down:
    // GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;
    // GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_1; // Select pull-down

    // 4. Configure EXTI for PC13
    // Select Port C for EXTI line 13. EXTICR[3] (or EXTICR4) handles EXTI lines 12-15.
    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13_Msk; // Clear current EXTI13 source
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;   // Set Port C as source for EXTI13

    // Configure EXTI line 13 properties
    EXTI->IMR1 |= EXTI_IMR1_IM13;    // Unmask (enable) interrupt on EXTI line 13
    EXTI->RTSR1 |= EXTI_RTSR1_RT13;   // Configure Rising Edge Trigger for EXTI line 13
    // EXTI->FTSR1 &= ~EXTI_FTSR1_FT13; // Ensure Falling Edge Trigger is disabled (usually default)

    // 5. Configure NVIC (Nested Vectored Interrupt Controller)
    NVIC_SetPriority(EXTI15_10_IRQn, 0); // Optional: Set interrupt priority (0 is often highest)
    NVIC_EnableIRQ(EXTI15_10_IRQn);      // Enable EXTI15_10_IRQn, which covers EXTI line 13

    // 6. Configure TIM2 for debounce timing (one-pulse mode)
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    TIM2->PSC     = (SystemCoreClock / 1000U) - 1U;
    TIM2->ARR     = DEBOUNCE_DELAY_MS - 1U;
    TIM2->CR1    |= TIM_CR1_OPM;
    TIM2->SR     &= ~TIM_SR_UIF;
    TIM2->DIER   |= TIM_DIER_UIE;
    NVIC_SetPriority(TIM2_IRQn, 1);
    NVIC_EnableIRQ(TIM2_IRQn);

    // Global interrupt enable (if not already enabled by startup code)
    // __enable_irq(); // CMSIS intrinsic

    // Main loop: The CPU can now perform other tasks or sleep.
    while (1) {
        // Enter a low-power mode, waiting for an interrupt
        __WFI(); // Wait For Interrupt
    }

    return 0; // Should never be reached
}
