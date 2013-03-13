/*
 * @author
 * Copyright (C) 2012 Luis R. Hilario http://www.luisdigital.com
 *
 */

#include "cpu_lpc1000.h"
#include "ssp.h"

volatile uint32_t usTicks;

void SysTick_Handler(void) {
	usTicks++;
}

void Delay_Init(void) {
	SysTick_Config(SystemCoreClock / 1000000);
}

void Delay_us(int us) {
	usTicks = 0;
	while (usTicks < us);
}

char SPI(char TX_Data) {
	while ((LPC_SSP0->SR & (SSPSR_TNF | SSPSR_BSY)) != SSPSR_TNF);
	LPC_SSP0->DR = TX_Data;

	while ((LPC_SSP0->SR & (SSPSR_BSY | SSPSR_RNE)) != SSPSR_RNE);
	return LPC_SSP0->DR;
}
