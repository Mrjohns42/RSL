/****************************************************************************
 *   $Id:: dactest.c 6092 2011-01-06 22:55:04Z nxp12832                     $
 *   Project: NXP LPC17xx DAC example
 *
 *   Description:
 *     This file contains DAC test modules, main entry, to test DAC APIs.
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
#include "dac.h"
#include "dma.h"

extern volatile uint32_t DACDMA0Done, DACDMA1Done;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  uint32_t i = 0;
  volatile uint32_t regVal;
#if DAC_DMA_ENABLED
  uint32_t *src_addr;
#else
  uint32_t m;
#endif

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  /* Initialize DAC  */
  DACInit();

#if DAC_DMA_ENABLED
  src_addr  = (uint32_t *)DMA_SRC;
  /* Fill the pattern in the DMA source memory */
  for ( i = 0; i < DATA_LENGTH; i++ )
  {
	*src_addr = (uint32_t)((i<<6)|(DAC_BIAS));	/* pre format the data to DACR register */
	src_addr++;	
  }
  
  DMA_Init();

  /* on DMA channel 0, source is memory, Destination is DAC */
  /* Enable channel and IE bit */
  DMAChannel_Init( 0, M2P ); 
  LPC_DAC->CTRL |= (0x1<<3);	/* DMA enabled on DAC */

  LPC_GPDMACH0->CConfig = 0xC001|(0x00<<1)|(DMA_DAC<<6)|(0x01<<11);
  while ( 1 )
  {
	/* Wait for DMA0 to finish. */
	while ( !DACDMA0Done );
	DACDMA0Done = 0;
	DMAChannel_Init( 0, M2P ); 
	/* Set TC int mask, channel enabled, src peripheral is memory(0x0), 
	dst peripheral is DAC(9), and transfer type is the M2P(1) */
	LPC_GPDMACH0->CConfig = 0xC001|(0x00<<1)|(DMA_DAC<<6)|(0x01<<11);
	/* During the DMA transfer, a saw wave should be seen on the scope. */
  }
#else
  while ( 1 )
  {
	LPC_DAC->CR = (i << 6) | DAC_BIAS;
	i++;
	for(m = 1000; m > 1; m--);
	if ( i == DATA_LENGTH )
	{
	  i = 0;
	}
  }
#endif
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/

