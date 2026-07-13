#include "gpio.h"



void setMODER(GPIO_TypeDef *gpio, uint8_t pin, portConfig output){
    gpio->MODER &= ~ (0x3UL << (pin*2));
    gpio->MODER |=  (output << (pin*2));
}

void gpio_write_pin(GPIO_TypeDef *gpio, uint8_t pin, uint8_t state)
{
    if (state)
        gpio->BSRR = (1U << pin);        // set high
    else
        gpio->BSRR = (1U << (pin + 16)); // set low
}

void toggle(GPIO_TypeDef *gpio, uint8_t pin, uint32_t ms)
{
    if (gpio->IDR & (1U << pin))
        gpio->BSRR = (1U << (pin + 16)); // set low
    else
        gpio->BSRR = (1U << pin);        // set high
    
    delay_ms(ms);

}