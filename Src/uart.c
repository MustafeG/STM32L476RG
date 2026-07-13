#include "uart.h"

// #define UART_BAUDRATE   115200U
#define UART_BAUDRATE   9600U   // Default in teraterm
#define USART2_CLK      4000000

#define RX_BUFFER_SIZE 128

static volatile char rx_buffer[RX_BUFFER_SIZE];
static volatile uint16_t write_index = 0;
static volatile uint8_t rx_ready = 0;


void USART2_IRQHandler(void){
    if (USART2->ISR & USART_ISR_RXNE)   // RXNE
    {
		char c = USART2->RDR;

        if(c == '\r' || c == '\n'){
            rx_buffer[write_index] = '\0';
            // uart_send_string(rx_buffer);
            write_index = 0;
            rx_ready = 1;
        }
        else{
            if (write_index < RX_BUFFER_SIZE){
                rx_buffer[write_index] = c;
                write_index++;
                uart_send_char(c);
            }
            else{
                // buffer full, byte lost
            }
            

        }

        // uart_send_char('\n');
        // uart_send_char('\r');
        // uart_send_char('>');
        // uart_send_char(c);
    }
}

void uart_init(void)
{
    //	Enable clock access to GPIOA
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    //	Enable clock access to USART2
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    //	USART2_TX = AF7 = PA2 alternate function
    setMODER(GPIOA, 2, ALTERNATE);

    //	USART2_RX = AF7 = PA3 alternate function
    setMODER(GPIOA, 3, ALTERNATE);

    //	AF7 = 0111
    //	PA2 = bits 11-8
	GPIOA->AFR[0] &= ~(0xFU << 8);
	GPIOA->AFR[0] |=  (7U   << 8);

    //	PA3 = bits 15-12
	GPIOA->AFR[0] &= ~(0xFU << 12);
	GPIOA->AFR[0] |=  (7U   << 12);

    /**	
     * USART baud rate register can only be written when the USART is disabled (UE=0)
     * p. 1337 reference manual
     * USARTDIV = BRR = (clock frequency) / (baud rate)
     * (4 MHz) / (19200) = 208 = BRR 
     **/
    USART2->CR1 &= ~USART_CR1_UE;
	USART2->BRR = (USART2_CLK / UART_BAUDRATE);

    //	USART enable | Receiver enable | Transmitter enable
	USART2->CR1 = USART_CR1_RE | USART_CR1_TE;
	USART2->CR1 |= USART_CR1_UE;

    USART2->CR1 |= USART_CR1_RXNEIE;
    NVIC_EnableIRQ(USART2_IRQn);
    
}


void uart_send_char(char c)
{
//	TXE: Transmit data register empty
    while (!(USART2->ISR & USART_ISR_TXE));  // TXE
    USART2->TDR = c;
}

void uart_send_newline(void)
{
    uart_send_char('\n');
    uart_send_char('\r');
}

void uart_send_string(const char *str)
{
    while (*str)
        uart_send_char(*str++);
}


uint8_t uart_rx_ready(void)
{
    return rx_ready;
}

void uart_clear_rx_ready(void)
{
    rx_ready = 0;
}

char* uart_get_rx_buffer(void)
{
    return (char*)rx_buffer;
}
