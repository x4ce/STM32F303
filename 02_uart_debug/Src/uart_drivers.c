#include	<stdint.h>
#include "stm32f3xx.h"

#define		EN_GPIOA		(1U<<17)
#define		EN_GPIOC		(1U<<19)
#define		EN_USART1		(1U<<14)
#define		EN_USART2		(1U<<17)
#define		CR1_UE			(1U<<0)
#define		CR1_RE			(1U<<2)
#define		CR1_TE			(1U<<3)
#define		USART_ISR_RXNE	(1U<<5)
#define		USART_ISR_TXE	(1U<<7)
#define		SYS_FREQ		8000000
#define		APB2_CLK		SYS_FREQ
#define		APB1_CLK		SYS_FREQ
#define		BAUDRATE		115200

typedef		uint8_t			usart_port;

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);


// Initialize UART1 at PC4, PC5 for debug
void uart1_debug_init(void)
{
	// Enable Clocks for GPIOC and USART1
	RCC->AHBENR |= EN_GPIOC;
	RCC->APB2ENR |= EN_USART1;

	// Enable PC4, PC5 as alternate function pins
	GPIOC->MODER |= (2<<8);
	GPIOC->MODER |= (2<<10);
	// Set alternate functions to TX, RX
	GPIOC->AFR[0] |= (7<<16);
	GPIOC->AFR[0] |= (7<<20);
	// Turn off USART1
	USART1->CR1 &= ~(CR1_UE);
	// Calculate baud rate
	USART1->BRR = compute_uart_bd(APB2_CLK, BAUDRATE);
	// Enable TE, RE
	USART1->CR1 |= (CR1_RE | CR1_TE);
	// Enable NVIC interrupt
	NVIC_EnableIRQ(USART1_IRQn);
	// Enable UE
	USART1->CR1 |= CR1_UE;
}

// Initialize UART2 at PA2, PA3 for debug
void uart2_init(void)
{
	// Enable Clocks for GPIOC and USART1
	RCC->AHBENR |= EN_GPIOA;
	RCC->APB1ENR |= EN_USART2;

	// Enable PA2, PA3 as alternate function pins
	GPIOA->MODER |= (2<<4);
	GPIOA->MODER |= (2<<6);
	// Set alternate functions to TX, RX
	GPIOA->AFR[0] |= (7<<8);
	GPIOA->AFR[0] |= (7<<12);
	// Turn off USART2
	USART2->CR1 &= ~(CR1_UE);
	// Calculate baud rate
	USART2->BRR = compute_uart_bd(APB1_CLK, BAUDRATE);
	// Enable TE, RE
	USART2->CR1 |= (CR1_RE | CR1_TE);
	// Enable NVIC interrupt
	NVIC_EnableIRQ(USART2_IRQn);
	// Enable UE
	USART2->CR1 |= CR1_UE;
}

// Computes baud rate for the USART
static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U))/baudrate);
}

// Reads Char from USART1
char read_uart(usart_port port)
{
	if (port == 0)
	{
	while(!(USART1->ISR & USART_ISR_RXNE)) {}
	return USART1->RDR;

	}
	else
	{
		return '\0';
	}
}

// Writes Char to USART1
void write_uart(char data, usart_port port)
{
	if (port == 0)
	{
	while(!(USART1->ISR & USART_ISR_TXE)) {}
	USART1->TDR = data;

	}

}

// Sends string to USART
void write_string_uart(const char *str, usart_port port)
{
	while(*str != '\0')
	{
	write_uart(*str++, port);

	}
}
