#ifndef UART_DRIVERS_H_
#define UART_DRIVERS_H_

typedef		uint8_t			usart_port;

void uart1_debug_init(void);
char read_uart(usart_port port);
void write_uart(char data, usart_port port);
void write_string_uart(const char *str, usart_port port);

#endif /* UART_DRIVERS_H_ */
