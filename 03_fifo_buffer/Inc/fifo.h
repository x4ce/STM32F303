#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stm32f3xx.h>

#define		UART_BUFFER_SIZE	512

typedef struct
{
	unsigned char buffer[UART_BUFFER_SIZE];
	uint32_t volatile head;
	uint32_t volatile tail;
} fifo_buffer;

void buffer_put(unsigned char c);
void buffer_put_string(const char *str);
unsigned char buffer_get(void);
int8_t is_response(char *str);


#endif /* FIFO_H_ */
