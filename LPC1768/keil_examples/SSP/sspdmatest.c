/****************************************************************************
 *   $Id:: sspdmatest.c 6098 2011-01-08 02:26:20Z nxp12832                  $
 *   Project: NXP LPC17xx SSP DMA example
 *
 *   Description:
 *     This file contains SSP DMA test modules, main entry.
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
#include "LPC17xx.h"                        /* LPC17xx definitions */
#include "sspdma.h"
#include "dma.h"

extern volatile uint32_t SSP0DMADone;
extern volatile uint32_t SSP1DMADone;
volatile uint8_t *u0_src, *u0_dst, *u1_src, *u1_dst;

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  uint32_t i, Dummy = Dummy;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  /* To test all DMA on all SSP ports, test software includes: 
  set SSP0 as DMA M2P, set SSP1 as DMA P2M, connect SSP0
  to SSP1 externally. Check content of SSP0 TX and SSP1 TX
  and monitor the data on the bus as well. */
  
  /* By default, SSP0 is master, SSP1 is slave. */ 
  SSP0Init();			/* initialize SSP port */
  SSP1Init();
  
  u0_src = (uint8_t *)SSP0_DMA_TX_SRC;
  u0_dst = (uint8_t *)SSP0_DMA_RX_DST;
  u1_src = (uint8_t *)SSP1_DMA_TX_SRC;
  u1_dst = (uint8_t *)SSP1_DMA_RX_DST;
  for ( i = 0; i < SSP_DMA_SIZE; i++ )
  {
	*u0_src++ = *u1_src++ = i;
	*u0_dst++ = *u1_dst++ = 0;
  }

  DMA_Init();

  /* Connect SSP0 and SSP1 externally. The sequence is, SSP DMA starts 
  from SSP0 TX, lookback to SSP1 RX using channel 0 and 1. Then, SSP DMA starts
  from SSP1 TX, lookback to SSP0 RX using channel 2 and 3.
  So, both M2P+P2M on SSP0 and SSP1 are tested. */

  /* DMA from SSP0 TX to SSP1 RX using channel 0 and 1. */
  DMAChannel_Init( 0, M2P );
  LPC_GPDMACH0->CConfig = 0x0C001|(0x00<<1)|(DMA_SSP0_TX<<6)|(0x01<<11);

  DMAChannel_Init( 1, P2M );
  LPC_GPDMACH1->CConfig = 0x0C001|(DMA_SSP1_RX<<1)|(0x00<<6)|(0x02<<11);
  /* Enable DMA RX on SSP1 */
  LPC_SSP1->DMACR = 0x01<<0;
  /* Enable DMA TX on SSP0 */
  LPC_SSP0->DMACR = 0x01<<1;

  while ( !SSP0DMADone );
  SSP0DMADone = 0;
  while ( !SSP1DMADone );
  SSP1DMADone = 0;

  u0_src = (uint8_t *)SSP0_DMA_TX_SRC;
  u1_dst = (uint8_t *)SSP1_DMA_RX_DST;
  for ( i = 0; i < SSP_DMA_SIZE; i++ )
  {
	if ( *u0_src++ != *u1_dst++ )
	{
	  while ( 1 );
	}
  }

  /* Clear the RX FIFO is important when the SSPx change roles and direction. */
  for ( i = 0; i < FIFOSIZE; i++ )
  {
	Dummy = LPC_SSP0->DR;		/* clear the FIFO */
	Dummy = LPC_SSP1->DR;		/* clear the FIFO */
  }

  /* For below test, SSP1 needs to be master, SSP0 needs to be slave. */
  /* Slave mode */
  if ( LPC_SSP0->CR1 & SSPCR1_SSE )
  {
	/* The slave bit can't be set until SSE bit is zero. */
	LPC_SSP0->CR1 &= ~SSPCR1_SSE;
  }
  LPC_SSP0->CR1 = SSPCR1_MS;		/* Enable slave bit first */
  LPC_SSP0->CR1 |= SSPCR1_SSE;	/* Enable SSP0 as slave */

  if ( LPC_SSP1->CR1 & SSPCR1_SSE )
  {
	/* The slave bit can't be set until SSE bit is zero. */
	LPC_SSP1->CR1 &= ~SSPCR1_SSE;
  }
  LPC_SSP1->CR1 = 0;		/* Disable slave bit first */
  LPC_SSP1->CR1 |= SSPCR1_SSE;	/* Enable SSP1 as master */

  /* DMA from SSP1 TX to SSP0 RX using channel 0 and 1. */
  DMAChannel_Init( 1, M2P );
  LPC_GPDMACH1->CConfig = 0x0C001|(0x00<<1)|(DMA_SSP1_TX<<6)|(0x01<<11);

  DMAChannel_Init( 0, P2M );
  LPC_GPDMACH0->CConfig = 0x0C001|(DMA_SSP0_RX<<1)|(0x00<<6)|(0x02<<11);
  /* Enable DMA RX on SSP0 */
  LPC_SSP0->DMACR = 0x01<<0;
  /* Enable DMA TX on SSP1 */
  LPC_SSP1->DMACR = 0x01<<1;

  while ( !SSP1DMADone );
  SSP1DMADone = 0;
  while ( !SSP0DMADone );
  SSP0DMADone = 0;

  u1_src = (uint8_t *)SSP1_DMA_TX_SRC;
  u0_dst = (uint8_t *)SSP0_DMA_RX_DST;
  for ( i = 0; i < SSP_DMA_SIZE; i++ )
  {
	if ( *u1_src++ != *u0_dst++ )
	{
	  while ( 1 );
	}
  }
  while ( 1 );	/* Never exit from main, easy for debugging. */
}

/******************************************************************************
**                            End Of File
******************************************************************************/

