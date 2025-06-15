#include <stdint.h>

/* Correct RCC base + AHB2ENR offset */
#define RCC_AHB2ENR   (*(volatile uint32_t*)(0x40021000U + 0x4CU))

/* GPIOA base + MODER/ODR offsets (these were already right) */
#define GPIOA_MODER   (*(volatile uint32_t*)(0x48000000U + 0x00U))
#define GPIOA_ODR     (*(volatile uint32_t*)(0x48000000U + 0x14U))

#define PIN5          (1U << 5)

static void delay(volatile uint32_t d) {
    while (d--) {
        __asm__("nop");
    }
}

int main(void) {
    /* 1) Enable GPIOA clock (bit 0 of AHB2ENR) */
    RCC_AHB2ENR |= (1U << 0);

    /* 2) Configure PA5 as output */
    GPIOA_MODER &= ~(3U << (5 * 2));
    GPIOA_MODER |=  (1U << (5 * 2));

    /* 3) Blink loop */
    for (;;) {
        GPIOA_ODR ^= PIN5;
        delay(1000000);
    }
}

