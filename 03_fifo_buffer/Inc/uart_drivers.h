#ifndef UART_DRIVERS_H_
#define UART_DRIVERS_H_

typedef		uint8_t			usart_port;

void uart1_debug_init(void);
void uart2_init(void);
char read_uart(usart_port port);
void write_uart(char data, usart_port port);
void write_string_uart(const char *str, usart_port port);
void systick_delay_ms(uint32_t delay);

#endif /* UART_DRIVERS_H_ */
