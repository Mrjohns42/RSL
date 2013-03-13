/*
 * @author
 * Copyright (C) 2012 Luis R. Hilario http://www.luisdigital.com
 *
 */

#include "LPC11xx.h"

/* Port 2, bit 0 is set to GPIO output (NRF24L01 CE) */
#define NRF24L01_CE_OUT		LPC_GPIO2->DIR |= 1;

#define NRF24L01_CE_HIGH	LPC_GPIO2->MASKED_ACCESS[1] = 1
#define NRF24L01_CE_LOW		LPC_GPIO2->MASKED_ACCESS[1] = 0
#define NRF24L01_CSN_HIGH	LPC_GPIO0->MASKED_ACCESS[(1<<2)] = 1<<2
#define NRF24L01_CSN_LOW	LPC_GPIO0->MASKED_ACCESS[(1<<2)] = 0

void Delay_Init(void);
void Delay_us(int us);
char SPI(char TX_Data);
