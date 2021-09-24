#include "stm32f3xx.h"

#define		EN_GPIOE		(1U<<21)


int main(void)
{
	// Enable clock to GPIOE
	RCC->AHBENR |= EN_GPIOE;
	// Set PE8 as output
	GPIOE->MODER |= (1U<<16);
    /* Loop forever */
	while(1)
	{
		GPIOE->BSRR |= (1U<<8);
		for (int i=0; i<500000; i++) {}
		GPIOE->BSRR |= (1U<<24);
		for (int i=0; i<500000; i++) {}

	}
}
