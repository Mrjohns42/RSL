/*
 * @author
 * Copyright (C) 2012 Luis R. Hilario http://www.luisdigital.com
 *
 */

#include "LPC11xx.h"

/* Port 0, bit 3 is set to GPIO output (NRF24L01 CE) */
#define NRF24L01_CE_OUT		LPC_GPIO0->DIR |= (1<<3);		  
#define NRF24L01_CSN_OUT    LPC_GPIO0->DIR |= (1<<2);

#define NRF24L01_CE_HIGH	LPC_GPIO0->MASKED_ACCESS[(1<<3)] = 1<<3
#define NRF24L01_CE_LOW		LPC_GPIO0->MASKED_ACCESS[(1<<3)] = 0
#define NRF24L01_CSN_HIGH	LPC_GPIO0->MASKED_ACCESS[(1<<2)] = 1<<2
#define NRF24L01_CSN_LOW	LPC_GPIO0->MASKED_ACCESS[(1<<2)] = 0

void Delay_Init(void);
void Delay_us(int us);
char SPI(char TX_Data);
