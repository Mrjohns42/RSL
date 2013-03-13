/****************************************************************************
 *   $Id:: sspdma.c 5804 2010-12-04 00:32:12Z usb00423                      $
 *   Project: NXP LPC17xx SSP DMA example
 *
 *   Description:
 *     This file contains SSP DMA code example which include SSP DMA
 *     initialization and APIs for SSP DMA access.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "LPC17xx.h"			/* LPC17xx Peripheral Registers */
#include "sspdma.h"

/*****************************************************************************
** Function name:		SSPInit
**
** Descriptions:		SSP port initialization routine
**				
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void SSP0Init( void )
{
  uint8_t i, Dummy=Dummy;

  /* Enable AHB clock to the SSP0. */
  LPC_SC->PCONP |= (0x1<<21);

  /* Further divider is needed on SSP0 clock. Using default divided by 4 */
  LPC_SC->PCLKSEL1 &= ~(0x3<<10);
  
  /* P0.15~0.18 as SSP0 */
  LPC_PINCON->PINSEL0 &= ~(0x3UL<<30);
  LPC_PINCON->PINSEL0 |= (0x2UL<<30);
  LPC_PINCON->PINSEL1 &= ~((0x3<<0)|(0x3<<2)|(0x3<<4));
  LPC_PINCON->PINSEL1 |= ((0x2<<0)|(0x2<<2)|(0x2<<4));

  /* Set DSS data to 8-bit, Frame format SPI, CPOL = 0, CPHA = 0, and SCR is 15 */
  LPC_SSP0->CR0 = 0x0707;

  /* SSPCPSR clock prescale register, master mode, minimum divisor is 0x02 */
  LPC_SSP0->CPSR = 0x2;

  for ( i = 0; i < FIFOSIZE; i++ )
  {
	Dummy = LPC_SSP0->DR;		/* clear the RxFIFO */
  }
	
  /* Device select as master, SSP Enabled */
#if SSP_SLAVE
  /* Slave mode */
  if ( LPC_SSP0->CR1 & SSPCR1_SSE )
  {
	/* The slave bit can't be set until SSE bit is zero. */
	LPC_SSP0->CR1 &= ~SSPCR1_SSE;
  }
  LPC_SSP0->CR1 = SSPCR1_MS;		/* Enable slave bit first */
  LPC_SSP0->CR1 |= SSPCR1_SSE;	/* Enable SSP */
#else
  /* Master mode */
  LPC_SSP0->CR1 = SSPCR1_SSE;
#endif
  return;
}

/*****************************************************************************
** Function name:		SSPInit
**
** Descriptions:		SSP port initialization routine
**				
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void SSP1Init( void )
{
  uint8_t i, Dummy=Dummy;

  /* Enable AHB clock to the SSP1. */
  LPC_SC->PCONP |= (0x1<<10);

  /* Further divider is needed on SSP1 clock. Using default divided by 4 */
  LPC_SC->PCLKSEL0 &= ~(0x3<<20);

  /* P0.6~0.9 as SSP1 */
  LPC_PINCON->PINSEL0 &= ~((0x3<<12)|(0x3<<14)|(0x3<<16)|(0x3<<18));
  LPC_PINCON->PINSEL0 |= ((0x2<<12)|(0x2<<14)|(0x2<<16)|(0x2<<18));
  
  /* Set DSS data to 8-bit, Frame format SPI, CPOL = 0, CPHA = 0, and SCR is 15 */
  LPC_SSP1->CR0 = 0x0707;

  /* SSPCPSR clock prescale register, master mode, minimum divisor is 0x02 */
  LPC_SSP1->CPSR = 0x2;

  for ( i = 0; i < FIFOSIZE; i++ )
  {
	Dummy = LPC_SSP1->DR;		/* clear the RxFIFO */
  }

  /* Device select as master, SSP Enabled */
#if SSP1_SLAVE
  /* Slave mode */
  if ( LPC_SSP1->CR1 & SSPCR1_SSE )
  {
	/* The slave bit can't be set until SSE bit is zero. */
	LPC_SSP1->CR1 &= ~SSPCR1_SSE;
  }
  LPC_SSP1->CR1 = SSPCR1_MS;		/* Enable slave bit first */
  LPC_SSP1->CR1 |= SSPCR1_SSE;	/* Enable SSP */
#else
  /* Master mode */
  LPC_SSP1->CR1 = SSPCR1_SSE;
#endif
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

