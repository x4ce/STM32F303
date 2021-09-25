#include <stdio.h>
#include "stm32f3xx.h"
#include "string.h"
#include "uart_drivers.h"
#include "fifo.h"


#define		EN_GPIOE		(1U<<21)

usart_port	db_port = 0;
usart_port	bt_port = 1;

int main(void)
{

	uart1_debug_init();
	uart2_init();

	//buffer_put_string("IsSm!!");

	for (int i=0; i<1500000; i++) {}

	while(1)
	{
		//char data[80];
		//sprintf(data, "Hello Brother\r\n");
		//write_string_uart(data, bt_port);
		//for (int i=0; i<1500000; i++) {}
		//unsigned char c = buffer_get();
		//write_uart(c, db_port);
		//result = is_response("Shahid");
		if (is_response("Shahid") ==1)
		{
			unsigned char data[80];
			sprintf(data, "Found the string!!!\r\n");
			write_string_uart(data, db_port);
		}
		else
		{
			unsigned char data[80];
			sprintf(data, "String not found...\r\n");
			write_string_uart(data, db_port);
		}

		systick_delay_ms(1000);
	}
}
