#include "systick.h"

static volatile uint32_t ms_ticks = 0;

void systick_init(void)
{
    // 4MHz/X = 1 kHz (1ms)
    // X = 4MHz/1kHz = 4000
    // 4MHz clock / 4000 = 1000 Hz = 1ms tick

    SysTick->LOAD = 4000 - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk    |
                    SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
    ms_ticks++;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = ms_ticks;
    while ((ms_ticks - start) < ms);
}

// returns current ms_ticks value
uint32_t get_ticks(void)
{
    return ms_ticks;
}        

// how many ms since a recorded start time
uint32_t elapsed_ms(uint32_t start)
{
    return (ms_ticks - start);

}

