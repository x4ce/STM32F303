#include "string.h"
#include "fifo.h"
#include "uart_drivers.h"


#define 	INIT_VAL			0
#define		UART_BUFFER_SIZE	512
#define		MAX_TRIES			10
#define		TRY_TIME			10

static int8_t response_wait(int8_t section);

int8_t curr_try = 0;

//fifo_buffer	tx_buffer1 = {{INIT_VAL}, INIT_VAL, INIT_VAL};
fifo_buffer	rx_buffer = {{INIT_VAL}, INIT_VAL, INIT_VAL};
//fifo_buffer	tx_buffer2 = {{INIT_VAL}, INIT_VAL, INIT_VAL};
//fifo_buffer	rx_buffer2 = {{INIT_VAL}, INIT_VAL, INIT_VAL};

//fifo_buffer *_tx_buffer1 = &tx_buffer1;
fifo_buffer *_rx_buffer = &rx_buffer;
//fifo_buffer *_tx_buffer2 = &tx_buffer2;
//fifo_buffer *_rx_buffer2 = &rx_buffer2;

// Function to wait for a response and return error code
static int8_t response_wait(int8_t section)
{
	if (curr_try == MAX_TRIES)
	{
		curr_try = 0;
		return section;
	}
	// Wait for sometime to receive data
	systick_delay_ms(TRY_TIME);
	curr_try++;
	return 1;
}

void buffer_put(unsigned char c)
{
	int newhead = (uint32_t)(((_rx_buffer->head) + 1) % UART_BUFFER_SIZE);

	if (newhead != _rx_buffer->tail)
	{
		_rx_buffer->buffer[_rx_buffer->head] = c;
		_rx_buffer->head = newhead;
	}

}

void buffer_put_string(const char *str)
{
	while(*str != '\0')
	{
		buffer_put(*str++);
	}
}

unsigned char buffer_get(uint8_t inc_tail)
{
	while((_rx_buffer->head) == (_rx_buffer->tail))
	{
		if ((response_wait(2)) != 1)
		{
			return 2;
		}
	}
	unsigned char c;
	c = _rx_buffer->buffer[_rx_buffer->tail];
	if (inc_tail == 1)
	{
		// if inc_tail is 1 then increment tail otherwise not
		_rx_buffer->tail = (uint32_t)(((_rx_buffer->tail) + 1) % UART_BUFFER_SIZE);
	}

	return c;
}

int8_t is_response(char *str)
{
	int curr_loc = 0;
	int len = strlen(str);

	while(!((_rx_buffer->head) == (_rx_buffer->tail)))
	{
		curr_loc = 0;
		// get the first character
		while(buffer_get(1) != str[0])
		{
			// Wait for more data
			while((_rx_buffer->head) == (_rx_buffer->tail))
			{
				//
				if ((response_wait(3)) != 1)
				{
					return 3;
				}
			}
		}

		curr_loc++;

		while(buffer_get(1) == str[curr_loc])
		{
			curr_loc++;
			// Wait for more data
			while((_rx_buffer->head) == (_rx_buffer->tail))
			{
				if ((response_wait(4)) != 1)
				{
					return 4;
				}
			}
		}

		if (len == curr_loc)
		{
			return 1;
		}

	}

	if (curr_loc == len)
	{
		// Success
		return 1;
	}
	else
	{
		// Fail
		return -1;
	}

}

// Function to process copy
static int8_t process_copy(char *str, char *dest_buffer, int start_pos)
{
	int8_t check; 					// flag for recursive process copy function
	int curr_pos = 0;
	int len = strlen(str);
	int indx = start_pos;

	// If data is there otherwise wait...
	while((_rx_buffer->head) == (_rx_buffer->tail))
	{
		if ((response_wait(4)) != 1)
		{
			return 4;
		}
	}

	while(buffer_get(0) != str[curr_pos])
	{
		dest_buffer[indx] = buffer_get(1);

		//_rx_buffer1->tail = (uint16_t)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE;

		indx++;

		// If data is there otherwise wait...
		while((_rx_buffer->head) == (_rx_buffer->tail))
		{
			if ((response_wait(5)) != 1)
			{
				return 5;
			}
		}

	}

	while(buffer_get(0) == str[curr_pos])
	{
		curr_pos++;
		dest_buffer[indx++] = buffer_get(1);
		if (curr_pos == len)
		{
			// Success
			return 1;
		}

		// If data is there otherwise wait...
		while((_rx_buffer->head) == (_rx_buffer->tail))
		{
			if ((response_wait(6)) != 1)
			{
				return 6;
			}
		}
	}

	if (curr_pos != len)
	{
		curr_pos = 0;

		check = process_copy(str, dest_buffer, indx);
	}

	// check for recursive process call
	if (check == 1)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

// Function to process copy up to a string
int8_t copy_up_to_string(char *str, char *dest_buffer)
{
	// Process copy
	return (process_copy(str, dest_buffer, 0));
}
