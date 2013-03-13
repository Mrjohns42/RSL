/*
 * @author
 * Copyright (C) 2012 Luis R. Hilario http://www.luisdigital.com
 *
 */

#include "cpu_lpc1000.h"
#include "lpc17xx_ssp.h"

uint32_t usTicks;

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

void SSPInit(void) {
	SSP_CFG_Type sspChannelConfig;

	NRF24L01_CE_OUT;
	NRF24L01_CE_LOW;

	LPC_PINCON->PINSEL0 |= 0x2<<14; //SCK1
	LPC_PINCON->PINSEL0 |= 0x2<<16; //MISO1
	LPC_PINCON->PINSEL0 |= 0x2<<18;	//MOSI1
	LPC_GPIO0->FIODIR 	|= 1<<6;	//P0.6 GPIO CSN

	NRF24L01_CSN_HIGH;

	SSP_ConfigStructInit(&sspChannelConfig);
	SSP_Init(LPC_SSP1, &sspChannelConfig);
	SSP_Cmd(LPC_SSP1, ENABLE);
}

char SPI(char TX_Data) {
	while ((LPC_SSP1->SR & (SSP_SR_TNF | SSP_SR_BSY)) != SSP_SR_TNF);
	LPC_SSP1->DR = TX_Data;

	while ((LPC_SSP1->SR & (SSP_SR_BSY | SSP_SR_RNE)) != SSP_SR_RNE);
	return LPC_SSP1->DR;
}
