#include "fifo.h"

#define 	INIT_VAL		0
#define		UART_BUFFER_SIZE	512

//fifo_buffer	tx_buffer1 = {{INIT_VAL}, INIT_VAL, INIT_VAL};
fifo_buffer	rx_buffer = {{INIT_VAL}, INIT_VAL, INIT_VAL};
//fifo_buffer	tx_buffer2 = {{INIT_VAL}, INIT_VAL, INIT_VAL};
//fifo_buffer	rx_buffer2 = {{INIT_VAL}, INIT_VAL, INIT_VAL};

//fifo_buffer *_tx_buffer1 = &tx_buffer1;
fifo_buffer *_rx_buffer = &rx_buffer;
//fifo_buffer *_tx_buffer2 = &tx_buffer2;
//fifo_buffer *_rx_buffer2 = &rx_buffer2;

void buffer_put(unsigned char c)
{
	int newhead = (uint32_t)(((_rx_buffer->head) + 1) % UART_BUFFER_SIZE);

	if (newhead != _rx_buffer->tail)
	{
		_rx_buffer->buffer[_rx_buffer->head] = c;
		_rx_buffer->head = newhead;
	}

}

unsigned char buffer_get(void)
{
	while((_rx_buffer->head) == (_rx_buffer->tail)) {}
	unsigned char c;
	c = _rx_buffer->buffer[_rx_buffer->tail];
	_rx_buffer->tail = (uint32_t)(((_rx_buffer->tail) + 1) % UART_BUFFER_SIZE);
	return c;
}


/*
void buffer_put(unsigned char c, fifo_buffer *mybuffer)
{
	int newhead = (uint32_t)(((mybuffer->head) + 1) % UART_BUFFER_SIZE);

	if (newhead != mybuffer->tail)
	{
		mybuffer->buffer[mybuffer->head] = c;
		mybuffer->head = newhead;
	}

}

*/


/*
unsigned char buffer_get(fifo_buffer *mybuffer)
{
	while((mybuffer->head) == (mybuffer->tail)) {}
	unsigned char c;
	c = mybuffer->buffer[mybuffer->tail];
	mybuffer->tail = (mybuffer->tail) + 1;
	return c;
}
*/
