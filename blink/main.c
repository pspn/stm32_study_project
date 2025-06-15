#include <stdint.h>

/* RCC AHB2 peripheral clock enable register */
#define RCC_AHB2ENR (*(volatile uint32_t*)0x4002104C)

/* GPIOA registers (LED บน Nucleo-64 ต่อที่ PA5) */
#define GPIOA_MODER (*(volatile uint32_t*)0x48000000)
#define GPIOA_ODR   (*(volatile uint32_t*)0x48000014)

#define PIN5        (1U << 5)

static void delay(volatile uint32_t d) {
    while (d--) {
        __asm("nop");
    }
}

int main(void) {
    /* 1) เปิด clock ให้ GPIOA (AHB2ENR bit0) */
    RCC_AHB2ENR |= (1U << 0);

    /* 2) ตั้ง PA5 เป็น general-purpose output (MODER5 = 01) */
    GPIOA_MODER &= ~(3U << (5 * 2));
    GPIOA_MODER |=  (1U << (5 * 2));

    /* 3) กระพริบ LED */
    while (1) {
        GPIOA_ODR |=  PIN5;     /* ON */
        delay(500000);
        GPIOA_ODR &= ~PIN5;     /* OFF */
        delay(500000);
    }
}

