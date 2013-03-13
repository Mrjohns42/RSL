/****************************************************************************
 *   $Id:: dmatest.c 6097 2011-01-07 04:31:25Z nxp12832                     $
 *   Project: NXP LPC17xx DMA example
 *
 *   Description:
 *     This file contains DMA test modules, main entry, to test DMA APIs.
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
#include "lpc17xx.h"
#include "type.h"
#include "dma.h"

extern volatile uint32_t DMATCCount;

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{	
  uint32_t i;
  uint32_t *src_addr, *dest_addr;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();
  
  src_addr = (uint32_t *)DMA_SRC;
  dest_addr = (uint32_t *)DMA_DST;
  for ( i = 0; i < DMA_SIZE/4; i++ )
  {
	*src_addr++ = i;
	*dest_addr++ = 0;
  }
	    
  DMA_Init( M2M );
  LPC_GPDMACH0->CConfig |= 0x08001;	/* Enable channel and IE bit */

  while ( !DMATCCount );		/* Wait until DMA is done */

  /* Verify result */
  src_addr = (uint32_t *)DMA_SRC;
  dest_addr = (uint32_t *)DMA_DST;
  for ( i = 0; i < DMA_SIZE/4; i++ )
  {
	if ( *src_addr++ != *dest_addr++ )
	{
	  while ( 1 );	/* fatal error */
	}
  }
  while ( 1 );	/* Done here, never exit from main for easier debugging. */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
