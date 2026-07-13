#ifndef GPIO_H
#define GPIO_H

#include "stm32l476xx.h"
#include <stdint.h>
#include "systick.h"

typedef enum{
    INPUT       = 0x0UL,
    OUTPUT      = 0x1UL,
    ALTERNATE   = 0x2UL,
    ANALOG      = 0x3UL
}portConfig;


void setMODER(GPIO_TypeDef *gpio, uint8_t pin, portConfig output);
void gpio_write_pin(GPIO_TypeDef *gpio, uint8_t pin, uint8_t state);
void toggle(GPIO_TypeDef *gpio, uint8_t pin, uint32_t ms);

#endif // GPIO_H