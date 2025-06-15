#include <stdint.h>

/* RCC base + AHB2ENR offset */
#define RCC_AHB2ENR (*(volatile uint32_t*)(0x40021000U + 0x4CU))

/* GPIOA (LED) */
#define GPIOA_MODER (*(volatile uint32_t*)(0x48000000U + 0x00U))
#define GPIOA_ODR   (*(volatile uint32_t*)(0x48000000U + 0x14U))

/* GPIOC (BUTTON) */
#define GPIOC_MODER (*(volatile uint32_t*)(0x48000800U + 0x00U))
#define GPIOC_PUPDR (*(volatile uint32_t*)(0x48000800U + 0x0CU))
#define GPIOC_IDR   (*(volatile uint32_t*)(0x48000800U + 0x10U))

#define PIN5   (1U << 5)   // PA5
#define PIN13  (1U << 13)  // PC13

static void delay(volatile uint32_t d) {
    while (d--) __asm__("nop");
}

int main(void) {
    /* 1) Enable GPIOA (bit0) and GPIOC (bit2) clocks */
    RCC_AHB2ENR |= (1U << 0) | (1U << 2);

    /* 2) PA5 = output */
    GPIOA_MODER &= ~(3U << (5 * 2));
    GPIOA_MODER |=  (1U << (5 * 2));

    /* 3) PC13 = input, pull-up */
    GPIOC_MODER &= ~(3U << (13 * 2));       // input
    GPIOC_PUPDR &= ~(3U << (13 * 2));
    GPIOC_PUPDR |=  (1U << (13 * 2));       // pull-up

    /* 4) Poll button, toggle LED on falling edge */
    uint32_t prev = GPIOC_IDR & PIN13;
    while (1) {
        uint32_t curr = GPIOC_IDR & PIN13;
        if (!(curr) && prev) {              // 1 → 0
            GPIOA_ODR ^= PIN5;              // toggle LD2
            delay(50000);                   // debounce
            while (!(GPIOC_IDR & PIN13));   // wait for release
        }
        prev = curr;
    }
}

