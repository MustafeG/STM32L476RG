#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include "stm32l476xx.h"

void systick_init(void);
void delay_ms(uint32_t ms);
uint32_t get_ticks(void);
uint32_t elapsed_ms(uint32_t start);

#endif // SYSTICK_H