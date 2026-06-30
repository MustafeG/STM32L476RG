#include "stm32l476xx.h"

void SystemInit(void)
{
}

void delay(volatile uint32_t count)
{
    while (count--);
}

int main(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    (void)RCC->AHB2ENR;

    GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
    GPIOA->MODER |=  GPIO_MODER_MODE5_0;

    while (1)
    {
        GPIOA->BSRR = GPIO_BSRR_BS5;
        delay(1000000);
        GPIOA->BSRR = GPIO_BSRR_BR5;
        delay(1000000);
    }
}