/****************************************************************************
 *   $Id:: dma.c 5795 2010-12-03 00:02:09Z usb00423                         $
 *   Project: NXP LPC17xx DMA example
 *
 *   Description:
 *     This file contains DMA code example which include DMA 
 *     initialization, DMA interrupt handler, and APIs for DMA
 *     transfer.
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
#include "LPC17xx.h"
#include "type.h"                      
#include "dac.h"
#include "dma.h"

#if DAC_DMA_ENABLED

volatile uint32_t DMATCCount = 0;
volatile uint32_t DMAErrCount = 0;
volatile uint32_t DACDMA0Done = 0;
volatile uint32_t DACDMA1Done = 0;

/******************************************************************************
** Function name:		DMA_IRQHandler
**
** Descriptions:		DMA interrupt handler
**
** parameters:		None
** Returned value:	None
** 
******************************************************************************/
void DMA_IRQHandler(void) 
{
  uint32_t regVal;

  regVal = LPC_GPDMA->IntTCStat;
  if ( regVal )
  {
	DMATCCount++;
	LPC_GPDMA->IntTCClear = regVal;
	if ( regVal & 0x01 )
	{
	  DACDMA0Done = 1;
	}
	else if ( regVal & 0x02 )
	{
	  DACDMA1Done = 1;
	}
  } 

  regVal = LPC_GPDMA->IntErrStat;
  if ( regVal )
  {
	DMAErrCount++;
	LPC_GPDMA->IntErrClr = regVal;
  }
  return;
}

/******************************************************************************
** Function name:		DMA_Init
**
** Descriptions:		clock to GPDMA in PCONP, GPDMA init before channel init		
**
** parameters:			None			
** Returned value:		None		
** 
******************************************************************************/
void DMA_Init( void )
{
  /* Enable CLOCK into GPDMA controller */
  LPC_SC->PCONP |= (1 << 29);

  /* sync enabled */
  LPC_GPDMA->Sync = 0xFFFF;

  LPC_SC->DMAREQSEL = 0x0000;

  LPC_GPDMA->IntTCClear = 0x03;
  LPC_GPDMA->IntErrClr = 0x03;

  LPC_GPDMA->Config = 0x01;	/* Enable DMA channels, little endian */
  while ( !(LPC_GPDMA->Config & 0x01) );

  NVIC_EnableIRQ(DMA_IRQn);
  return;
}

/******************************************************************************
** Function name:		DMAChannel_Init
**
** Descriptions:		
**
** parameters:			Channel number, DMA mode: M2P, P2M, M2M, P2P			
** Returned value:		TRUE or FALSE
** 
******************************************************************************/
uint32_t DMAChannel_Init( uint32_t ChannelNum, uint32_t DMAMode )
{
  if ( ChannelNum == 0 )
  {
	DACDMA0Done = 0;
	/* Clear DMA channel 0 terminal count. */
	LPC_GPDMA->IntTCClear = 0x01;
	LPC_GPDMA->IntErrClr = 0x01;
	LPC_GPDMACH0->CControl = 0;
	LPC_GPDMACH0->CConfig = 0;   
	if ( DMAMode == M2P )
	{
	  /* Ch0 set for M2P transfer from mempry to DAC. */
	  LPC_GPDMACH0->CSrcAddr = DMA_SRC;
	  LPC_GPDMACH0->CDestAddr = DMA_DST;
	  /* The burst size is set to 1, source and dest transfer width is
	  32 bits(word), src addr increment by 1. dst addr unchange. Terminal 
	  Count Int enable */
	  LPC_GPDMACH0->CControl = (DMA_SIZE & 0x0FFF)|(0x00<<12) 
		|(0x00<<15)|(0x02<<18)|(0x02<<21)|(1<<26)|(0<<27)|0x80000000;
	  return( TRUE );
	}
	else
	{
	  return ( FALSE );
	}
  }
  else if ( ChannelNum == 1 )
  {
	DACDMA1Done = 0;
	/* Clear DMA channel 1 terminal count. */
	LPC_GPDMA->IntTCClear = 0x02;
	LPC_GPDMA->IntErrClr = 0x02;
	LPC_GPDMACH1->CControl = 0;
	LPC_GPDMACH1->CConfig = 0;      
	if ( DMAMode == M2P )
	{
	  /* Ch1 set for M2P transfer from mempry to DAC. */
	  LPC_GPDMACH1->CSrcAddr = DMA_SRC;
	  LPC_GPDMACH1->CDestAddr = DMA_DST;
	  /* The burst size is set to 1, source and dest transfer width is
	  32 bits(word), src addr increment by 1. dst addr unchange. Terminal 
	  Count Int enable */
	  LPC_GPDMACH1->CControl = (DMA_SIZE & 0x0FFF)|(0x00<<12) 
		|(0x00<<15)|(0x02<<18)|(0x02<<21)|(1<<26)|(0<<27)|0x80000000;
	  return( TRUE );
	}
	else
	{
	  return ( FALSE );
	}
  }
  else
  {
	return ( FALSE );
  }
}

#endif	/* end if DMA_ENABLED */

/******************************************************************************
**                            End Of File
******************************************************************************/
