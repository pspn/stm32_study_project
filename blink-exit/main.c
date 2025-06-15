#include "stm32l476xx.h"

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR1 & EXTI_PR1_PIF13) {
        EXTI->PR1 = EXTI_PR1_PIF13;
        GPIOA->ODR ^= GPIO_ODR_OD5;
    }
}

int main(void) {
    /* 1) Enable GPIOA, GPIOC, SYSCFG clocks */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* 2) PA5 as output */
    GPIOA->MODER  = (GPIOA->MODER & ~GPIO_MODER_MODE5_Msk)
                  |  (1U << GPIO_MODER_MODE5_Pos);
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
    GPIOA->OSPEEDR|=  GPIO_OSPEEDR_OSPEED5;
    GPIOA->PUPDR  &= ~GPIO_PUPDR_PUPD5;

    /* 3) PC13 as input (external pull-up on Nucleo) */
    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;

    /* 4) Route PC13→EXTI13 */
    SYSCFG->EXTICR[3] = (SYSCFG->EXTICR[3] & ~SYSCFG_EXTICR4_EXTI13)
                      |  SYSCFG_EXTICR4_EXTI13_PC;

    /* 5) EXTI13: unmask & falling edge */
    EXTI->IMR1  |= EXTI_IMR1_IM13;
    EXTI->FTSR1 |= EXTI_FTSR1_FT13;
    EXTI->RTSR1 &= ~EXTI_RTSR1_RT13;
    EXTI->PR1   = EXTI_PR1_PIF13;  // clear any pending

    /* 6) Enable in NVIC */
    NVIC_SetPriority(EXTI15_10_IRQn, 2);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    /* 7) Sleep until interrupt */
    while (1) {
        __WFI();
    }
}

