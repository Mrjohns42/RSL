/*
 * @author
 * Copyright (C) 2012 Luis R. Hilario http://www.luisdigital.com
 *
 */

#include "LPC17xx.h"

/* Port 0, bit 17 is set to GPIO output (NRF24L01 CE) */
#define NRF24L01_CE_OUT		LPC_GPIO0->FIODIR |= (1<<17)

#define NRF24L01_CE_HIGH	LPC_GPIO0->FIOSET |= (1<<17)
#define NRF24L01_CE_LOW		LPC_GPIO0->FIOCLR |= (1<<17)
#define NRF24L01_CSN_HIGH	LPC_GPIO0->FIOSET |= (1<<6)
#define NRF24L01_CSN_LOW	LPC_GPIO0->FIOCLR |= (1<<6)

void Delay_Init(void);
void Delay_us(int us);
void SSPInit(void);
char SPI(char TX_Data);
