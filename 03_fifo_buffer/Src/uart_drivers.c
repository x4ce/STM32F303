#include	<stdint.h>
#include "stm32f3xx.h"
#include "fifo.h"

#define		EN_GPIOA			(1U<<17)
#define		EN_GPIOC			(1U<<19)
#define		EN_USART1			(1U<<14)
#define		EN_USART2			(1U<<17)
#define		CR1_UE				(1U<<0)
#define		CR1_RE				(1U<<2)
#define		CR1_TE				(1U<<3)
#define		CR1_RXNEIE			(1U<<5)
#define		CR1_TXEIE			(1U<<7)
#define		ISR_RXNE			(1U<<5)
#define		ISR_TXE				(1U<<7)
#define		SYS_FREQ			8000000
#define		APB2_CLK			SYS_FREQ
#define		APB1_CLK			SYS_FREQ
#define		DB_BAUDRATE			9600
#define		BT_BAUDRATE			9600
#define		SYSTICK_LOAD_VAL	8000
#define		CTRL_ENABLE			(1U<<0)
#define		CTRL_CLKSRC			(1U<<2)
#define		CTRL_COUNTERFLAG	(1U<<16)

typedef		uint8_t			usart_port;

usart_port	debug_port = 0;
usart_port	bluetooth_port = 1;


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
	USART1->BRR = compute_uart_bd(APB2_CLK, DB_BAUDRATE);
	// Enable TE, RE
	USART1->CR1 |= (CR1_RE | CR1_TE);
	// Enable NVIC interrupt
	NVIC_EnableIRQ(USART1_IRQn);
	// Enable RX interrupt
	USART1->CR1 |= CR1_RXNEIE;
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
	USART2->BRR = compute_uart_bd(APB1_CLK, BT_BAUDRATE);
	// Enable TE, RE
	USART2->CR1 |= (CR1_RE | CR1_TE);
	// Enable NVIC interrupt
	NVIC_EnableIRQ(USART2_IRQn);
	// Enable RX interrupt
	USART2->CR1 |= CR1_RXNEIE;
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
	while(!(USART1->ISR & ISR_RXNE)) {}
	return USART1->RDR;

	}
	else if (port == 1)
	{
		while(!(USART2->ISR & ISR_RXNE)) {}
		return USART2->RDR;
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
	while(!(USART1->ISR & ISR_TXE)) {}
	USART1->TDR = data;

	}

	if (port == 1)
	{
	while(!(USART2->ISR & ISR_TXE)) {}
	USART2->TDR = data;

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


void systick_delay_ms(uint32_t delay)
{
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	SysTick->VAL = 0;

	SysTick->CTRL = CTRL_CLKSRC | CTRL_ENABLE;

	for (int i = 0; i < delay; i++)
	{
		while((SysTick->CTRL & CTRL_COUNTERFLAG) == 0){}
	}

	SysTick->CTRL = 0;
}

// Interrupt handler for USART1
void USART1_EXTI25_IRQHandler(void)
{
	// Check if USART1 RXN interrupt
	if (((USART1->ISR & ISR_RXNE) !=0) && ((USART1->CR1 & CR1_RXNEIE) !=0))
	{
		unsigned char c;
		// Receive data
		c = USART1->RDR;
		// Echo data to bluetooth port
		write_uart(c, bluetooth_port);
	}

}

// Interrupt handler for USART2
void USART2_EXTI26_IRQHandler(void)
{
	// Check if USART2 RXN interrupt
	if (((USART2->ISR & ISR_RXNE) !=0) && ((USART2->CR1 & CR1_RXNEIE) !=0))
	{
		unsigned char c;
		//Receive data
		c = USART2->RDR;
		// Echo data to debug port
		//write_uart(c, debug_port);
		buffer_put(c);
	}
	}


