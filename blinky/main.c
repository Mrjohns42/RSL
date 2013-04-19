#include <stdio.h>
#include "LPC11xx.h"

void configureGPIO()
{
	//enable clocks to GPIO block
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  6);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  16);

	//set port 0_7 to output (high current drain in LPC1114)
    LPC_GPIO0->DIR |= (1<<7);
}

void ledOn()
{
	LPC_GPIO0->DATA &= ~(1<<7);
}

void ledOff()
{						 
	LPC_GPIO0->DATA |= (1<<7);
}

int main()
{
	int i;
	configureGPIO();
	while (1)
	{
		ledOn();
		for (i = 0; i < 0x000FFFFF; i++)
		{
		}
		ledOff();
		for (i = 0; i < 0x000FFFFF; i++)
		{
		}
	}
}
