#ifndef UART_H
#define UART_H

// Code Here
#include "stm32l476xx.h"
#include <stdint.h>
#include "gpio.h"

void uart_init(void);
void uart_send_char(char c);
void uart_send_newline(void);
void uart_send_string(const char *str);

uint8_t uart_rx_ready(void);
void uart_clear_rx_ready(void);
char* uart_get_rx_buffer(void);

#endif // UART_H