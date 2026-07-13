#include "stm32l476xx.h"
#include "gpio.h"
#include "systick.h"
#include "uart.h"

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
    uart_init();

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
        toggle(GPIOA, 5, 1000);
        // uart_send_char('M');
        if (uart_rx_ready())
        {
            uart_send_newline();
            uart_send_char('>');
            uart_send_string(uart_get_rx_buffer());
            uart_send_newline();
            uart_clear_rx_ready();
        }
    }
}