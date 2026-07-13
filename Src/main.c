#include "stm32l476xx.h"
#include "gpio.h"
#include "systick.h"

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

    systick_init();

    setMODER(GPIOA, 5, OUTPUT);


    // Red LED = PC10
    // White LED = PC12
    // Green LED = PC11
    // USER_LED = PA5 = pin 21

    // RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    // setMODER(GPIOC, 10, OUTPUT);
    // setMODER(GPIOC, 11, OUTPUT);
    // setMODER(GPIOC, 12, OUTPUT);

    while (1)
    {
        // GPIOA->BSRR = GPIO_BSRR_BS5;
        // delay(1000000);
        // GPIOA->BSRR = GPIO_BSRR_BR5;
        // delay(1000000);

        // void toggle(GPIO_TypeDef *gpio, uint8_t pin, uint32_t ms)
        toggle(GPIOA, 5, 3000);
    }
}