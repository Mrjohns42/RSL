/****************************************************************************
 *   $Id:: dma.c 5804 2010-12-04 00:32:12Z usb00423                         $
 *   Project: NXP LPC17xx SSP DMA example
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
#include "lpc17xx.h"
#include "type.h"
#include "dma.h"                      
#include "sspdma.h"

volatile uint32_t DMATCCount = 0;
volatile uint32_t DMAErrCount = 0;
volatile uint32_t SSP0DMADone = 0;
volatile uint32_t SSP1DMADone = 0;
volatile uint32_t SSP2DMADone = 0;
volatile uint32_t SSP3DMADone = 0;

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
	if ( regVal & (0x01<<0) )
	{
	  SSP0DMADone = 1;
	}
	else if ( regVal & (0x01<<1) )
	{
	  SSP1DMADone = 1;
	}
	else if ( regVal & (0x01<<2) )
	{
	  SSP2DMADone = 1;
	}
	else if ( regVal & (0x01<<3) )
	{
	  SSP3DMADone = 1;
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

  /* Select primary function(SSP0/1/2) in DMA channels */
  LPC_SC->DMAREQSEL = 0x0000;

  /* Clear DMA channel 0~3 interrupts. */
  LPC_GPDMA->IntTCClear = 0x0F;
  LPC_GPDMA->IntErrClr = 0x0F;

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
	SSP0DMADone = 0;
	LPC_GPDMA->IntTCClear = 0x01<<0;
	LPC_GPDMA->IntErrClr = 0x01<<0;   
	if ( DMAMode == M2P )
	{
	  /* Ch0 set for M2P transfer from mempry to peripheral. */
	  LPC_GPDMACH0->CSrcAddr = SSP0_DMA_TX_SRC;
	  LPC_GPDMACH0->CDestAddr = SSP0_DMA_TX_DST;
	  /* The burst size is set to 1, source and dest transfer width is
	  32 bits(word), Terminal Count Int enable */
	  LPC_GPDMACH0->CControl = (SSP_DMA_SIZE & 0x0FFF)|(0x00 << 12) 
		|(0x00 << 15)|(0x00 << 18)|(0x00 << 21)|(1 << 26)|0x80000000;
	}
	else if ( DMAMode == P2M )
	{
	  /* Ch0 set for P2M transfer from peripheral to memory. */
	  LPC_GPDMACH0->CSrcAddr = SSP0_DMA_RX_SRC;
	  LPC_GPDMACH0->CDestAddr = SSP0_DMA_RX_DST;
	  /* The burst size is set to 1. Terminal Count Int enable. */
	  LPC_GPDMACH0->CControl = (SSP_DMA_SIZE & 0x0FFF)|(0x00 << 12) 
		|(0x00 << 15)|(0x00 << 18)|(0x00 << 21)|(1 << 27)|0x80000000;
	}
	else
	{
	  return ( FALSE );
	}
  }
  else if ( ChannelNum == 1 )
  {
	SSP1DMADone = 0;   
	LPC_GPDMA->IntTCClear = 0x01<<1;
	LPC_GPDMA->IntErrClr = 0x01<<1;
	if ( DMAMode == M2P )
	{
	  /* Ch1 set for M2P transfer from mempry to peripheral. */
	  LPC_GPDMACH1->CSrcAddr = SSP1_DMA_TX_SRC;
	  LPC_GPDMACH1->CDestAddr = SSP1_DMA_TX_DST;
	  /* The burst size is set to 1. Terminal Count Int enable. */
	  LPC_GPDMACH1->CControl = (SSP_DMA_SIZE & 0x0FFF)|(0x00 << 12) 
		|(0x00 << 15)|(0x00 << 18)|(0x00 << 21)|(1 << 26)|0x80000000;
	}
	else if ( DMAMode == P2M )
	{
	  /* Ch1 set for P2M transfer from peripheral to memory. */
	  LPC_GPDMACH1->CSrcAddr = SSP1_DMA_RX_SRC;
	  LPC_GPDMACH1->CDestAddr = SSP1_DMA_RX_DST;
	  /* The burst size is set to 1. Terminal Count Int enable. */
	  LPC_GPDMACH1->CControl = (SSP_DMA_SIZE & 0x0FFF)|(0x00 << 12) 
		|(0x00 << 15)|(0x00 << 18)|(0x00 << 21)|(1 << 27)|0x80000000;
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
  return( TRUE );
}

/******************************************************************************
**                            End Of File
******************************************************************************/
