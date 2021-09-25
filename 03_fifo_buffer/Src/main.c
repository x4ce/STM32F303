#include <stdio.h>
#include "stm32f3xx.h"
#include "uart_drivers.h"
#include "string.h"

#define		EN_GPIOE		(1U<<21)

usart_port	db_port = 0;
usart_port	bt_port = 1;

int main(void)
{

	uart1_debug_init();
	uart2_init();

	while(1)
	{
		//char data[80];
		//sprintf(data, "Hello Brother\r\n");
		//write_string_uart(data, bt_port);
		//for (int i=0; i<500000; i++) {}

	}
}
